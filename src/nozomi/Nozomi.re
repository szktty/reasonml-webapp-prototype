open Kanae;

module Error = {
  module Basic = {
    type t;
  };
  type t;
};

module Request = {
  module Basic = {
    type t;
  };
  type t = Basic.t;
  let from = (js: Basic.t) : t => js;
};

module Response = {
  module Basic = {
    type t;
    [@bs.send]
    external render :
      (t, string, Js.t('locals), ('error, string) => unit) => unit =
      "render";
    [@bs.send] external send : (t, string) => unit = "send";
  };
  type t = Basic.t;
  let from = (js: Basic.t) : t => js;
  let render =
      (res, ~view, ~locals, ~f: (~error: 'error, ~html: string) => unit) =>
    Basic.render(res, view, locals, (error, html) => f(~error, ~html));
  let send = Basic.send;
};

module Router = {
  module Basic = {
    type t;
    type handler = (Request.Basic.t, Response.Basic.t, unit => unit) => unit;
    [@bs.module "express"] external create : unit => t = "Router";
    [@bs.send] external use : (t, string, handler) => unit = "use";
  };
  type t = Basic.t;
  type handler = (Request.t, Response.t, unit => unit) => unit;
  let create = Basic.create;
  let use = (router, ~path, ~f) => Basic.use(router, path, f);
};

module App = {
  module Basic = {
    type t;
    type renderer = (unit /* error */, string) => unit;
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
        ~path,
        app,
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
  module type Locals = {type locals;};
  module type S = {
    type locals;
    type t;
    let create: string => t;
    let render:
      (t, ~response: Response.t, ~locals: Js.t(locals), ~f: handler) => unit;
  };
  module Make = (L: Locals) : (S with type locals := L.locals) => {
    type locals = L.locals;
    type t = {path__: string /* avoid duplicating name in locals */};
    let create = path => {path__: path};
    let render = (view, ~response, ~locals, ~f) => {
      Js.log("# View.render");
      Response.render(response, ~view=view.path__, ~locals, ~f);
    };
  };
};

let express = App.create;