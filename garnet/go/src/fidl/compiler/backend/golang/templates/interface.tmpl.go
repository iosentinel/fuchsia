// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package templates

const Interface = `
{{- define "InterfaceDefinition" -}}

const (
{{- range .Methods }}
	{{- range .Ordinals.Reads }}
	{{ .Name }} uint64 = {{ .Ordinal }}
	{{- end }}
{{- end }}
)

type {{ .ProxyName }} _bindings.{{ .ProxyType }}
{{ range .Methods }}
{{range .DocComments}}
//{{ . }}
{{- end}}
func (p *{{ $.ProxyName }}) {{ if .IsEvent -}}
		{{ .EventExpectName }}
	{{- else -}}
		{{ .Name }}
	{{- end -}}
	(
	{{- if .Request -}}
	{{- range $index, $m := .Request.Members -}}
		{{- if $index -}}, {{- end -}}
		{{ $m.PrivateName }} {{ $m.Type }}
	{{- end -}}
	{{- end -}}
	)
	{{- if .HasResponse -}}
	{{- if .Response }} (
	{{- range .Response.Members }}{{ .Type }}, {{ end -}}
		error)
	{{- else }} error{{ end -}}
	{{- else }} error{{ end }} {

	{{- if .HasRequest }}
	{{- if .Request }}
	req_ := &{{ .Request.Name }}{
		{{- range .Request.Members }}
		{{ .Name }}: {{ .PrivateName }},
		{{- end }}
	}
	{{- else }}
	var req_ _bindings.Message
	{{- end }}
	{{- end }}
	{{- if .HasResponse }}
	{{- if .Response }}
	resp_ := &{{ .Response.Name }}{}
	{{- else }}
	var resp_ _bindings.Message
	{{- end }}
	{{- end }}
	{{- if .HasRequest }}
		{{- if .HasResponse }}
	err_ := ((*_bindings.{{ $.ProxyType }})(p)).Call({{ .Ordinals.Write.Name }}, req_, resp_
		{{- range $index, $ordinal := .Ordinals.Reads -}}, {{ $ordinal.Name }}{{- end -}})
		{{- else }}
	err_ := ((*_bindings.{{ $.ProxyType }})(p)).Send({{ .Ordinals.Write.Name }}, req_)
		{{- end }}
	{{- else }}
		{{- if .HasResponse }}
	err_ := ((*_bindings.{{ $.ProxyType }})(p)).Recv(
		{{- with $first_ordinal := index .Ordinals.Reads 0 -}}
			{{- $first_ordinal.Name -}}
		{{- end -}}
		, resp_
		{{- range $index, $ordinal := .Ordinals.Reads -}}
			{{- if $index -}}, {{ $ordinal.Name }}{{- end -}}
		{{- end -}}
		)
		{{- else }}
	err_ := nil
		{{- end }}
	{{- end }}
	{{- if .HasResponse }}
	{{- if .Response }}
	return {{ range .Response.Members }}resp_.{{ .Name }}, {{ end }}err_
	{{- else }}
	return err_
	{{- end }}
	{{- else }}
	return err_
	{{- end }}
}
{{- end }}

{{range .DocComments}}
//{{ . }}
{{- end}}
type {{ .WithCtxName }} interface {
{{- range .Methods }}
	{{- range .DocComments}}
	//{{ . }}
	{{- end}}
	{{- if .HasRequest }}
	{{- if .Request }}
	{{ .Name }}(ctx_ _bindings.Context
	{{- range .Request.Members -}}
		, {{ .PrivateName }} {{ .Type }}
	{{- end -}}
	)
	{{- else }}
	{{ .Name }}(ctx_ _bindings.Context)
	{{- end }}
	{{- if .HasResponse -}}
	{{- if .Response }} (
	{{- range .Response.Members }}{{ .Type }}, {{ end -}}
		error)
	{{- else }} error{{ end -}}
	{{- else }} error{{ end }}
	{{- end }}
{{- end }}
}

{{ $transitionalBaseWithCtxName := .TransitionalBaseWithCtxName }}

type {{.TransitionalBaseWithCtxName}} struct {}

{{ range .Methods }}
{{- if and .IsTransitional .Request }}
func (_ *{{$transitionalBaseWithCtxName}}) {{ .Name }} (ctx_ _bindings.Context
{{- range .Request.Members -}}
	, {{ .PrivateName }} {{ .Type }}
{{- end -}}
)
{{- if .HasResponse -}}
	{{- if .Response }} (
		{{- range .Response.Members }}{{ .Type }}, {{ end -}}
			error)
	{{- else -}}
		error
	{{ end -}}
{{- else -}}
	error
{{- end -}}
{
	panic("Not Implemented")
}
{{- end }}
{{- end }}

{{range .DocComments}}
//{{ . }}
{{- end}}
type {{ .Name }} interface {
{{- range .Methods }}
	{{- range .DocComments}}
	//{{ . }}
	{{- end}}
	{{- if .HasRequest }}
	{{- if .Request }}
	{{ .Name }}(
	{{- range $index, $m := .Request.Members -}}
		{{- if $index -}}, {{- end -}}
		{{ $m.PrivateName }} {{ $m.Type }}
	{{- end -}}
	)
	{{- else }}
	{{ .Name }}()
	{{- end }}
	{{- if .HasResponse -}}
	{{- if .Response }} (
	{{- range .Response.Members }}{{ .Type }}, {{ end -}}
		error)
	{{- else }} error{{ end -}}
	{{- else }} error{{ end }}
	{{- end }}
{{- end }}
}

type {{.TransitionalBaseName}} struct {}

{{ $transitionalBaseName := .TransitionalBaseName }}

{{- range  $method := .Methods }}
	{{- if $method.IsTransitional }}
		{{- if $method.HasRequest }}
			{{- if $method.Request }}
			func (_ *{{$transitionalBaseName}}) {{ $method.Name }} (
			{{- range $index, $m := $method.Request.Members -}}
				{{- if $index -}}, {{- end -}}
				{{ $m.PrivateName }} {{ $m.Type }}
			{{- end -}}
			)
			{{- else }}
			func (_ *{{$transitionalBaseName}}) {{ $method.Name }} ()
			{{- end }}
			{{- if $method.HasResponse -}}
				{{- if $method.Response }} (
					{{- range $method.Response.Members }}{{ .Type }}, {{ end -}}
						error)
				{{- else -}}
					error
				{{ end -}}
			{{- else -}}
				error
			{{- end -}}
			{
				panic("Not Implemented")
			}
		{{- end -}}
	{{- end}}
{{- end }}

{{- if eq .ProxyType "ChannelProxy" }}
type {{ .RequestName }} _bindings.InterfaceRequest

func New{{ .RequestName }}() ({{ .RequestName }}, *{{ .ProxyName }}, error) {
	req, cli, err := _bindings.NewInterfaceRequest()
	return {{ .RequestName }}(req), (*{{ .ProxyName }})(cli), err
}

{{- if .ServiceNameString }}
// Implements ServiceRequest.
func (_ {{ .RequestName }}) Name() string {
	return {{ .ServiceNameString }}
}
func (c {{ .RequestName }}) ToChannel() _zx.Channel {
	return c.Channel
}

const {{ .ServiceNameConstant }} = {{ .ServiceNameString }}
{{- end }}
{{- end }}

type {{ .StubName }} struct {
	Impl {{ .Name }}
}

func (s_ *{{ .StubName }}) DispatchImpl(ctx_ _bindings.Context, ordinal_ uint64, data_ []byte, handles_ []_zx.Handle) (_bindings.Message, bool, error) {
	switch ordinal_ {
	{{- range .Methods }}
	{{- if not .IsEvent }}
	{{- range $index, $ordinal := .Ordinals.Reads }}
		{{- if $index }}
		fallthrough
		{{- end }}
	case {{ $ordinal.Name }}:
	{{- end }}
		{{- if .HasRequest }}
		{{- if .Request }}
		in_ := {{ .Request.Name }}{}
		if _, _, err_ := _bindings.UnmarshalWithContext(ctx_.GetMarshalerContext(), data_, handles_, &in_); err_ != nil {
			return nil, false, err_
		}
		{{- end }}
		{{- end }}
		{{ if .Response }}
		{{- range .Response.Members }}{{ .PrivateName }}, {{ end -}}
		{{- end -}}
		err_ := s_.Impl.{{ .Name }}(
		{{- if .HasRequest }}
		{{- if .Request -}}
		{{- range $index, $m := .Request.Members -}}
		{{- if $index -}}, {{- end -}}
		in_.{{ $m.Name }}
		{{- end -}}
		{{- end -}}
		{{- end -}}
		)
		{{- if .HasResponse }}
		{{- if .Response }}
		out_ := {{ .Response.Name }}{}
		{{- range .Response.Members }}
		out_.{{ .Name }} = {{ .PrivateName }}
		{{- end }}
		return &out_, true, err_
		{{- else }}
		return nil, true, err_
		{{- end }}
		{{- else }}
		return nil, false, err_
		{{- end }}
	{{- end }}
	{{- end }}
	}
	return nil, false, _bindings.ErrUnknownOrdinal
}

func (s_ *{{ .StubName }}) DispatchImplWithCtx(ordinal_ uint64, ctx_ _bindings.MarshalerContext, data_ []byte, handles_ []_zx.Handle) (_bindings.Message, bool, error) {
	switch ordinal_ {
	{{- range .Methods }}
	{{- if not .IsEvent }}
	{{- range $index, $ordinal := .Ordinals.Reads }}
		{{- if $index }}
		fallthrough
		{{- end }}
	case {{ $ordinal.Name }}:
	{{- end }}
		{{- if .HasRequest }}
		{{- if .Request }}
		in_ := {{ .Request.Name }}{}
		if _, _, err_ := _bindings.UnmarshalWithContext(ctx_, data_, handles_, &in_); err_ != nil {
			return nil, false, err_
		}
		{{- end }}
		{{- end }}
		{{ if .Response }}
		{{- range .Response.Members }}{{ .PrivateName }}, {{ end -}}
		{{- end -}}
		err_ := s_.Impl.{{ .Name }}(
		{{- if .HasRequest -}}
		{{- if .Request -}}
		{{- range $index, $m := .Request.Members -}}
		{{- if $index -}}, {{- end -}}
		in_.{{ $m.Name }}
		{{- end -}}
		{{- end -}}
		{{- end -}}
		)
		{{- if .HasResponse }}
		{{- if .Response }}
		out_ := {{ .Response.Name }}{}
		{{- range .Response.Members }}
		out_.{{ .Name }} = {{ .PrivateName }}
		{{- end }}
		return &out_, true, err_
		{{- else }}
		return nil, true, err_
		{{- end }}
		{{- else }}
		return nil, false, err_
		{{- end }}
	{{- end }}
	{{- end }}
	}
	return nil, false, _bindings.ErrUnknownOrdinal
}

{{- if eq .ProxyType "ChannelProxy" }}
type {{ .ServerName }} struct {
	_bindings.BindingSet
}

func (s *{{ .ServerName }}) Add(impl {{ .Name }}, c _zx.Channel, onError func(error)) (_bindings.BindingKey, error) {
	return s.BindingSet.Add(&{{ .StubName }}{Impl: impl}, c, onError)
}

func (s *{{ .ServerName }}) EventProxyFor(key _bindings.BindingKey) (*{{ .EventProxyName }}, bool) {
	pxy, err := s.BindingSet.ProxyFor(key)
	return (*{{ .EventProxyName }})(pxy), err
}
{{- end }}

type {{ .EventProxyName }} _bindings.{{ .ProxyType }}
{{ range .Methods }}
{{- if .IsEvent }}
func (p *{{ $.EventProxyName }}) {{ .Name }}(
	{{- if .Response -}}
	{{- range $index, $m := .Response.Members -}}
		{{- if $index -}}, {{- end -}}
		{{ $m.PrivateName }} {{ $m.Type }}
	{{- end -}}
	{{- end -}}
	) error {

	{{- if .HasResponse }}
	{{- if .Response }}
	event_ := &{{ .Response.Name }}{
		{{- range .Response.Members }}
		{{ .Name }}: {{ .PrivateName }},
		{{- end }}
	}
	{{- else }}
	var event_ _bindings.Message
	{{- end }}
	{{- end }}
	return ((*_bindings.{{ $.ProxyType }})(p)).Send({{ .Ordinals.Write.Name }}, event_)
}
{{- end }}
{{- end }}

{{ end -}}
`
