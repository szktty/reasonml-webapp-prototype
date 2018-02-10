open Kanae;

module Error = {
  module Basic = {
    type t;
  };
  type t = Basic.t;
};

module Request = {
  module Basic = {
    type t;
    [@bs.get] external method_ : t => string = "method";
    [@bs.get] external originalURL : t => string = "originalUrl";
    [@bs.get] external path : t => string = "path";
    [@bs.get] external protocol : t => string = "protocol";
    [@bs.get] external query : t => Js.Json.t = "query";
    [@bs.get] external secure : t => Js.boolean = "secure";
  };
  type method =
    | Get
    | Post
    | Put
    | Delete;
  type protocol =
    | HTTP
    | HTTPS
    | Other(string);
  type t = Basic.t;
  let from = (js: Basic.t) : t => js;
  let method = Basic.method;
  let isSecure = req => Basic.secure(req) |> Js.to_bool;
  let originalURL = Basic.originalURL;
  let path = Basic.path;
  let protocol = req =>
    switch (Basic.protocol(req)) {
    | "http" => HTTP
    | "https" => HTTPS
    | other => Other(other)
    };
  let query = (req, path: string) : option(string) => {
    let empty = Js.Dict.empty() |> Js.Json.object_;
    let rec query0 = (comps, accu: Js.Json.t) : Js.Json.t =>
      switch comps {
      | [] => accu
      | [comp] =>
        switch (Js.Json.decodeObject(accu)) {
        | None => empty
        | Some(obj) =>
          switch (Js.Dict.get(obj, comp)) {
          | None => empty
          | Some(value) => value
          }
        }
      | [comp, ...comps] =>
        switch (Js.Json.decodeObject(accu)) {
        | None => empty
        | Some(obj) =>
          switch (Js.Dict.get(obj, comp)) {
          | None => empty
          | Some(value) => query0(comps, value)
          }
        }
      };
    switch (KanaeString.split(path, ~on='.')) {
    | [] => None
    | comps => query0(comps, Basic.query(req)) |> Js.Json.decodeString
    };
  };
};

module Response = {
  module Basic = {
    type t;
    [@bs.send]
    external render :
      (t, string, Js.t('locals), ('error, string) => [@bs.uncurry] unit) =>
      unit =
      "render";
    [@bs.send] external append : (t, string, string) => string = "append";
    [@bs.send] external get : t => string = "get";
    [@bs.send] external location : (t, string) => unit = "location";
    [@bs.send] external send : (t, string) => unit = "send";
    [@bs.send] external status : (t, int) => t = "status";
    [@bs.send] external type_ : (t, string) => string = "type";
  };
  type t = Basic.t;
  let from = (js: Basic.t) : t => js;
  let append = Basic.append;
  let location = Basic.location;
  let get = Basic.get;
  let render =
      (
        res,
        ~view,
        ~locals,
        ~f: (~error: 'error, ~html: string) => [@bs.uncurry] unit
      ) =>
    Basic.render(res, view, locals, (error, html) => f(~error, ~html));
  let send = Basic.send;
  let status = Basic.status;
  let type_ = Basic.type_;
};

module Router = {
  module Basic = {
    type t;
    type handler =
      (Request.Basic.t, Response.Basic.t, unit => [@bs.uncurry] unit) =>
      [@bs.uncurry] unit;
    [@bs.module "express"] external create : unit => t = "Router";
    [@bs.send] external use : (t, string, handler) => unit = "use";
  };
  type t = Basic.t;
  type handler =
    (
      ~request: Request.t,
      ~response: Response.t,
      ~next: unit => [@bs.uncurry] unit
    ) =>
    unit;
  let create = Basic.create;
  let use = (router, ~path, ~f) => Basic.use(router, path, f);
  /*
   let middleware0 =
       (router, ~request: Request.t, ~response: Response.t, ~next: unit => unit)
       : handler =>
     (~request, ~response, ~next) => ();
   let middleware: t => handler = router => middleware0(router);
   */
};

module App = {
  module Basic = {
    type t;
    type renderer = (unit /* error */, string) => [@bs.uncurry] unit;
    [@bs.val] external express : unit => t = "Express";
    [@bs.send]
    external render : (t, Js.t('local), renderer) => unit = "render";
    [@bs.send] external listenPath : (t, string) => unit = "listen";
    [@bs.send]
    external listenPort : (t, int, Js.null(string), Js.null(string)) => unit =
      "listen";
    [@bs.send] external set : (t, string, 'value) => unit = "set";
    [@bs.send]
    external use : (t, string, Router.Basic.handler) => unit = "use";
    [@bs.send]
    external get : (t, string, Router.Basic.handler) => unit = "get";
    [@bs.send]
    external post : (t, string, Router.Basic.handler) => unit = "post";
    [@bs.send]
    external put : (t, string, Router.Basic.handler) => unit = "put";
    [@bs.send]
    external delete : (t, string, Router.Basic.handler) => unit = "delete";
  };
  module ViewEngine = {
    type t =
      | Jade
      | Pug;
    let toJS = engine =>
      switch engine {
      | Jade => "jade"
      | Pug => "pug"
      };
  };
  module Settings = {
    type t =
      | Views(list(string))
      | ViewEngine(ViewEngine.t);
  };
  type t = Basic.t;
  let create = Basic.express;
  let set = (app, setting) =>
    switch setting {
    | Settings.Views(views) => Basic.set(app, "views", Js.fromList(views))
    | ViewEngine(engine) =>
      Basic.set(app, "view engine", ViewEngine.toJS(engine))
    };
  let listen = (app, ~path) => Basic.listenPath(app, path);
  let start = (~host=?, ~backlog=?, app, ~port: int) =>
    Basic.listenPort(
      app,
      port,
      Js.Null.from_opt(host),
      Js.Null.from_opt(backlog)
    );
  let use =
      (
        app,
        ~path,
        ~f:
           (~request: Request.t, ~response: Response.t, ~next: unit => unit) =>
           unit
      ) =>
    Basic.use(app, path, (req, res, next) =>
      f(~request=Request.from(req), ~response=Response.from(res), ~next)
    );
};

module View = {
  type handler = (~error: Error.t, ~html: string) => unit;
  /* avoid duplicating name in locals */
  type t = {path: string};
  module type Locals = {type locals;};
  module type S = {
    type locals;
    let create: string => t;
    let render:
      (t, ~response: Response.t, ~locals: Js.t(locals), ~f: handler) => unit;
  };
  module Make = (L: Locals) : (S with type locals := L.locals) => {
    type locals = L.locals;
    let create = path => {path: path};
    let render = (view, ~response, ~locals: Js.t(locals), ~f) =>
      Response.render(response, ~view=view.path, ~locals, ~f);
  };
};

module StaticOptions = {
  module Basic = {
    type t = {
      .
      "dotfiles": string,
      "etag": Js.boolean,
      "extensions": Js.Falseable.t(array(string)),
      "fallthrough": Js.boolean,
      "immutable": Js.boolean,
      "index": Js.Falseable.t(string),
      "lastModified": Js.boolean,
      "maxAge": int,
      "redirect": Js.boolean,
      "setHeaders": (Response.Basic.t, string, string) => [@bs.uncurry] unit
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
        (~response: Response.t, ~path: string, ~stat: string) =>
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
    "extensions": Js.Falseable.from(opts.extensions),
    "fallthrough": Js.fromBool(opts.fallthrough),
    "immutable": Js.fromBool(opts.immutable),
    "index": Js.Falseable.from(opts.index),
    "lastModified": Js.fromBool(opts.lastModified),
    "maxAge": opts.maxAge,
    "redirect": Js.fromBool(opts.redirect),
    "setHeaders":
      switch opts.setHeaders {
      | None => ((_res, _path, _stat) => ())
      | Some(f) => (
          (res, path, stat) => f(~response=Response.from(res), ~path, ~stat)
        )
      }
  };
};

module Basic = {
  type t;
  [@bs.send]
  external static : (t, string, StaticOptions.Basic.t) => t = "static";
};

let top = App.Basic.express;

let express = App.create;

let static = (top, root, opts: StaticOptions.t) =>
  Basic.static(top, root, StaticOptions.toBasic(opts));