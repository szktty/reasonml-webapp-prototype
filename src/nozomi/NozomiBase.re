open Kanae.Base;

module StaticOptions = {
  module Basic = {
    type t = {
      .
      "dotfiles": string,
      "etag": Js.Boolean.t,
      "extensions": Js.False.t(array(string)),
      "fallthrough": Js.Boolean.t,
      "immutable": Js.Boolean.t,
      "index": Js.False.t(string),
      "lastModified": Js.Boolean.t,
      "maxAge": int,
      "redirect": Js.Boolean.t,
      "setHeaders":
        (NozomiResponse.Basic.t, string, string) => [@bs.uncurry] unit
    };
  };
  type dotfiles =
    | Allow
    | Deny
    | Ignore;
  type t = {
    dotfiles,
    etag: bool,
    extensions: option(array(string)),
    fallthrough: bool,
    immutable: bool,
    index: option(string),
    lastModified: bool,
    maxAge: int,
    redirect: bool,
    setHeaders:
      option(
        (~response: NozomiResponse.t, ~path: string, ~stat: string) =>
        [@bs.uncurry] unit
      )
  };
  let create =
      (
        ~dotfiles=Ignore,
        ~etag=true,
        ~extensions=?,
        ~fallthrough=true,
        ~immutable=false,
        ~index=?,
        ~lastModified=true,
        ~maxAge=0,
        ~redirect=true,
        ~setHeaders=?,
        ()
      ) => {
    dotfiles,
    etag,
    extensions,
    fallthrough,
    immutable,
    index,
    lastModified,
    maxAge,
    redirect,
    setHeaders
  };
  let toBasic = (opts: t) => {
    "dotfiles":
      switch opts.dotfiles {
      | Allow => "allow"
      | Deny => "deny"
      | Ignore => "ignore"
      },
    "etag": Js.fromBool(opts.etag),
    "extensions": Js.False.from(opts.extensions),
    "fallthrough": Js.fromBool(opts.fallthrough),
    "immutable": Js.fromBool(opts.immutable),
    "index": Js.False.from(opts.index),
    "lastModified": Js.fromBool(opts.lastModified),
    "maxAge": opts.maxAge,
    "redirect": Js.fromBool(opts.redirect),
    "setHeaders":
      switch opts.setHeaders {
      | None => ((_res, _path, _stat) => ())
      | Some(f) => (
          (res, path, stat) =>
            f(~response=NozomiResponse.from(res), ~path, ~stat)
        )
      }
  };
};

module Basic = {
  type t;
  [@bs.send]
  external static : (t, string, StaticOptions.Basic.t) => t = "static";
};

let top = NozomiApp.Basic.express;

let express = NozomiApp.create;

let static = (top, root, opts: StaticOptions.t) =>
  Basic.static(top, root, StaticOptions.toBasic(opts));