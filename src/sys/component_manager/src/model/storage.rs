// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

use {
    crate::clonable_error::ClonableError,
    crate::model::*,
    cm_rust::CapabilityPath,
    failure::{Error, Fail},
    fidl::endpoints,
    fidl_fuchsia_io::{
        DirectoryMarker, DirectoryProxy, MODE_TYPE_DIRECTORY, OPEN_RIGHT_READABLE,
        OPEN_RIGHT_WRITABLE,
    },
    fidl_fuchsia_sys2 as fsys,
    std::{path::PathBuf, sync::Arc},
};

const FLAGS: u32 = OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE;

/// Errors related to isolated storage.
#[derive(Debug, Fail, Clone)]
pub enum StorageError {
    #[fail(
        display = "failed to open isolated storage from {}'s directory {} for {}: {} ",
        dir_source_moniker, dir_source_path, relative_moniker, err
    )]
    Open {
        dir_source_moniker: AbsoluteMoniker,
        dir_source_path: CapabilityPath,
        relative_moniker: RelativeMoniker,
        #[fail(cause)]
        err: ClonableError,
    },
    #[fail(
        display = "failed to remove isolated storage from {}'s directory {} for {}: {} ",
        dir_source_moniker, dir_source_path, relative_moniker, err
    )]
    Remove {
        dir_source_moniker: AbsoluteMoniker,
        dir_source_path: CapabilityPath,
        relative_moniker: RelativeMoniker,
        #[fail(cause)]
        err: ClonableError,
    },
    #[fail(display = "storage path for {} was invalid", relative_moniker)]
    InvalidStoragePath { relative_moniker: RelativeMoniker },
    #[fail(display = "invalid utf8")]
    InvalidUtf8,
}

impl StorageError {
    pub fn open(
        dir_source_moniker: AbsoluteMoniker,
        dir_source_path: CapabilityPath,
        relative_moniker: RelativeMoniker,
        err: impl Into<Error>,
    ) -> Self {
        Self::Open { dir_source_moniker, dir_source_path, relative_moniker, err: err.into().into() }
    }

    pub fn remove(
        dir_source_moniker: AbsoluteMoniker,
        dir_source_path: CapabilityPath,
        relative_moniker: RelativeMoniker,
        err: impl Into<Error>,
    ) -> Self {
        Self::Remove {
            dir_source_moniker,
            dir_source_path,
            relative_moniker,
            err: err.into().into(),
        }
    }

    pub fn invalid_storage_path(relative_moniker: RelativeMoniker) -> Self {
        Self::InvalidStoragePath { relative_moniker }
    }
}

/// Open the isolated storage sub-directory for the given component, creating it if necessary.
/// `dir_source_realm` and `dir_source_path` are the realm hosting the directory and its capability
/// path.
pub async fn open_isolated_storage(
    model: &Model,
    dir_source_realm: Arc<Realm>,
    dir_source_path: &CapabilityPath,
    storage_type: fsys::StorageType,
    relative_moniker: &RelativeMoniker,
    open_mode: u32,
) -> Result<DirectoryProxy, StorageError> {
    const FLAGS: u32 = OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE;
    let (dir_proxy, local_server_end) =
        endpoints::create_proxy::<DirectoryMarker>().expect("failed to create proxy");
    model
        .bind_instance_open_outgoing(
            dir_source_realm.clone(),
            FLAGS,
            open_mode,
            dir_source_path,
            local_server_end.into_channel(),
        )
        .await
        .map_err(|e| {
            StorageError::open(
                dir_source_realm.abs_moniker.clone(),
                dir_source_path.clone(),
                relative_moniker.clone(),
                e,
            )
        })?;
    let storage_proxy = io_util::create_sub_directories(
        &dir_proxy,
        &generate_storage_path(Some(storage_type), &relative_moniker),
    )
    .map_err(|e| {
        StorageError::open(
            dir_source_realm.abs_moniker.clone(),
            dir_source_path.clone(),
            relative_moniker.clone(),
            e,
        )
    })?;
    Ok(storage_proxy)
}

/// Delete the isolated storage sub-directory for the given component.  `dir_source_realm` and
/// `dir_source_path` are the realm hosting the directory and its capability path.
pub async fn delete_isolated_storage(
    model: &Model,
    dir_source_realm: Arc<Realm>,
    dir_source_path: &CapabilityPath,
    relative_moniker: &RelativeMoniker,
) -> Result<(), StorageError> {
    let (root_dir, local_server_end) =
        endpoints::create_proxy::<DirectoryMarker>().expect("failed to create proxy");
    model
        .bind_instance_open_outgoing(
            dir_source_realm.clone(),
            FLAGS,
            MODE_TYPE_DIRECTORY,
            dir_source_path,
            local_server_end.into_channel(),
        )
        .await
        .map_err(|e| {
            StorageError::open(
                dir_source_realm.abs_moniker.clone(),
                dir_source_path.clone(),
                relative_moniker.clone(),
                e,
            )
        })?;
    let storage_path = generate_storage_path(None, &relative_moniker);
    if storage_path.parent().is_none() {
        return Err(StorageError::invalid_storage_path(relative_moniker.clone()));
    }
    let dir_path = storage_path.parent().unwrap();
    let name = storage_path
        .file_name()
        .ok_or(StorageError::invalid_storage_path(relative_moniker.clone()))?
        .to_str()
        .ok_or(StorageError::InvalidUtf8)?;
    let dir = if dir_path.parent().is_none() {
        root_dir
    } else {
        io_util::open_directory(&root_dir, dir_path, FLAGS).map_err(|e| {
            StorageError::open(
                dir_source_realm.abs_moniker.clone(),
                dir_source_path.clone(),
                relative_moniker.clone(),
                e,
            )
        })?
    };

    // TODO(36377): This function is subject to races. If another process has a handle to the
    // isolated storage directory, it can add files while this function is running. That could
    // cause it to spin or fail because a subdir was not empty after it removed all the contents.
    // It's also possible that the directory was already deleted by the backing component or a
    // prior run.
    files_async::remove_dir_recursive(&dir, name).await.map_err(|e| {
        StorageError::remove(
            dir_source_realm.abs_moniker.clone(),
            dir_source_path.clone(),
            relative_moniker.clone(),
            e,
        )
    })?;
    Ok(())
}

/// Generates the path into a directory the provided component will be afforded for storage
///
/// The path of the sub-directory for a component that uses a storage capability is based on:
///
/// - Each component instance's child moniker as given in the `children` section of its parent's
///   manifest, for each component instance in the path from the [`storage`
///   declaration][storage-syntax] to the [`use` declaration][use-syntax].
/// - The storage type.
///
/// These names are used as path elements, separated by elements of the name "children". The
/// storage type is then appended to this path.
///
/// For example, if the following component instance tree exists, with `a` declaring storage
/// capabilities, and then cache storage being offered down the chain to `d`:
///
/// ```
///  a  <- declares storage, offers cache storage to b
///  |
///  b  <- offers cache storage to c
///  |
///  c  <- offers cache storage to d
///  |
///  d  <- uses cache storage as `/my_cache`
/// ```
///
/// When `d` attempts to access `/my_cache` the framework creates the sub-directory
/// `b:0/children/c:0/children/d:0/cache` in the directory used by `a` to declare storage
/// capabilities.  Then, the framework gives 'd' access to this new directory.
fn generate_storage_path(
    type_: Option<fsys::StorageType>,
    relative_moniker: &RelativeMoniker,
) -> PathBuf {
    assert!(
        !relative_moniker.down_path().is_empty(),
        "storage capability appears to have been exposed or used by its source"
    );

    let mut down_path = relative_moniker.down_path().iter();
    let mut dir_path = vec![down_path.next().unwrap().as_str().to_string()];
    while let Some(p) = down_path.next() {
        dir_path.push("children".to_string());
        dir_path.push(p.as_str().to_string());
    }
    match type_ {
        Some(fsys::StorageType::Data) => dir_path.push("data".to_string()),
        Some(fsys::StorageType::Cache) => dir_path.push("cache".to_string()),
        Some(fsys::StorageType::Meta) => dir_path.push("meta".to_string()),
        None => {}
    }
    dir_path.into_iter().collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    use {
        crate::{
            model::testing::routing_test_helpers::RoutingTest,
            model::testing::test_helpers::{self, default_component_decl},
        },
        cm_rust::*,
        fidl_fuchsia_sys2::StorageType,
        std::convert::{TryFrom, TryInto},
    };

    #[fuchsia_async::run_singlethreaded(test)]
    async fn open_isolated_storage_test() {
        let components = vec![
            (
                "a",
                ComponentDecl {
                    children: vec![
                        ChildDecl {
                            name: "b".to_string(),
                            url: "test:///b".to_string(),
                            startup: fsys::StartupMode::Lazy,
                        },
                        ChildDecl {
                            name: "c".to_string(),
                            url: "test:///c".to_string(),
                            startup: fsys::StartupMode::Lazy,
                        },
                    ],
                    ..default_component_decl()
                },
            ),
            (
                "b",
                ComponentDecl {
                    exposes: vec![ExposeDecl::Directory(ExposeDirectoryDecl {
                        source_path: "/data".try_into().unwrap(),
                        source: ExposeSource::Self_,
                        target_path: "/data".try_into().unwrap(),
                        target: ExposeTarget::Realm,
                    })],
                    ..default_component_decl()
                },
            ),
        ];
        let test = RoutingTest::new("a", components).await;
        let b_moniker = vec!["b:0"].into();
        let dir_source_realm = test.model.look_up_realm(&b_moniker).await.expect("lookup failed");
        let dir_source_path = CapabilityPath::try_from("/data").unwrap();
        let relative_moniker = RelativeMoniker::new(vec![], vec!["c:0".into(), "coll:d:1".into()]);

        // Open.
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Data,
            &relative_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, Vec::<String>::new());
        test_helpers::write_file(&dir, "file", "hippos").await;
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Open again.
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Data,
            &relative_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Open another component's storage.
        let relative_moniker =
            RelativeMoniker::new(vec![], vec!["c:0".into(), "coll:d:1".into(), "e:0".into()]);
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Data,
            &relative_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, Vec::<String>::new());
        test_helpers::write_file(&dir, "file", "hippos").await;
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Open a different storage type.
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Cache,
            &relative_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, Vec::<String>::new());
        test_helpers::write_file(&dir, "file", "hippos").await;
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Error -- tried to open nonexistent storage.
        let root_moniker = vec![].into();
        let root_realm = test.model.look_up_realm(&root_moniker).await.expect("lookup failed");
        let err = open_isolated_storage(
            &test.model,
            root_realm,
            &dir_source_path,
            fsys::StorageType::Data,
            &relative_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect_err("open isolated storage not meant to succeed");
        match err {
            StorageError::Open { .. } => {}
            _ => {
                panic!("unexpected error: {:?}", err);
            }
        }
    }

    #[fuchsia_async::run_singlethreaded(test)]
    async fn delete_isolated_storage_test() {
        let components = vec![
            (
                "a",
                ComponentDecl {
                    children: vec![
                        ChildDecl {
                            name: "b".to_string(),
                            url: "test:///b".to_string(),
                            startup: fsys::StartupMode::Lazy,
                        },
                        ChildDecl {
                            name: "c".to_string(),
                            url: "test:///c".to_string(),
                            startup: fsys::StartupMode::Lazy,
                        },
                    ],
                    ..default_component_decl()
                },
            ),
            (
                "b",
                ComponentDecl {
                    exposes: vec![ExposeDecl::Directory(ExposeDirectoryDecl {
                        source_path: "/data".try_into().unwrap(),
                        source: ExposeSource::Self_,
                        target_path: "/data".try_into().unwrap(),
                        target: ExposeTarget::Realm,
                    })],
                    ..default_component_decl()
                },
            ),
        ];
        let test = RoutingTest::new("a", components).await;
        let b_moniker = vec!["b:0"].into();
        let dir_source_realm = test.model.look_up_realm(&b_moniker).await.expect("lookup failed");
        let dir_source_path = CapabilityPath::try_from("/data").unwrap();
        let parent_moniker = RelativeMoniker::new(vec![], vec!["c:0".into()]);
        let child_moniker = RelativeMoniker::new(vec![], vec!["c:0".into(), "coll:d:1".into()]);

        // Open and write to all storage types in child.
        for storage_type in
            vec![fsys::StorageType::Data, fsys::StorageType::Cache, fsys::StorageType::Meta]
        {
            let dir = open_isolated_storage(
                &test.model,
                dir_source_realm.clone(),
                &dir_source_path,
                storage_type,
                &child_moniker,
                OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
            )
            .await
            .expect("failed to open isolated storage");
            assert_eq!(test_helpers::list_directory(&dir).await, Vec::<String>::new());
            test_helpers::write_file(&dir, "file", "hippos").await;
            assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);
        }

        // Open parent's storage.
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Data,
            &parent_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, Vec::<String>::new());
        test_helpers::write_file(&dir, "file", "hippos").await;
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Delete the child's storage.
        delete_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            &child_moniker,
        )
        .await
        .expect("failed to delete child's isolated storage");

        // Open parent's storage again. Should work.
        let dir = open_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            fsys::StorageType::Data,
            &parent_moniker,
            OPEN_RIGHT_READABLE | OPEN_RIGHT_WRITABLE,
        )
        .await
        .expect("failed to open isolated storage");
        assert_eq!(test_helpers::list_directory(&dir).await, vec!["file".to_string()]);

        // Open list of children from parent. Should not contain child directory.
        assert_eq!(
            test.list_directory_in_storage(parent_moniker.clone(), "children").await,
            Vec::<String>::new(),
        );

        // Error -- tried to delete nonexistent storage.
        let err = delete_isolated_storage(
            &test.model,
            dir_source_realm.clone(),
            &dir_source_path,
            &child_moniker,
        )
        .await
        .expect_err("delete isolated storage not meant to succeed");
        match err {
            StorageError::Remove { .. } => {}
            _ => {
                panic!("unexpected error: {:?}", err);
            }
        }
    }

    #[test]
    fn generate_storage_path_test() {
        for (type_, relative_moniker, expected_output) in vec![
            (
                Some(StorageType::Data),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1"]),
                ),
                "a:1/data",
            ),
            (
                Some(StorageType::Cache),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1"]),
                ),
                "a:1/cache",
            ),
            (
                Some(StorageType::Meta),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1"]),
                ),
                "a:1/meta",
            ),
            (
                None,
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1"]),
                ),
                "a:1",
            ),
            (
                Some(StorageType::Data),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2"]),
                ),
                "a:1/children/b:2/data",
            ),
            (
                Some(StorageType::Cache),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2"]),
                ),
                "a:1/children/b:2/cache",
            ),
            (
                Some(StorageType::Meta),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2"]),
                ),
                "a:1/children/b:2/meta",
            ),
            (
                None,
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2"]),
                ),
                "a:1/children/b:2",
            ),
            (
                Some(StorageType::Data),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2", "c:3"]),
                ),
                "a:1/children/b:2/children/c:3/data",
            ),
            (
                Some(StorageType::Cache),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2", "c:3"]),
                ),
                "a:1/children/b:2/children/c:3/cache",
            ),
            (
                Some(StorageType::Meta),
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2", "c:3"]),
                ),
                "a:1/children/b:2/children/c:3/meta",
            ),
            (
                None,
                RelativeMoniker::from_absolute(
                    &AbsoluteMoniker::from(vec![]),
                    &AbsoluteMoniker::from(vec!["a:1", "b:2", "c:3"]),
                ),
                "a:1/children/b:2/children/c:3",
            ),
        ] {
            assert_eq!(
                generate_storage_path(type_, &relative_moniker),
                PathBuf::from(expected_output)
            )
        }
    }
}
