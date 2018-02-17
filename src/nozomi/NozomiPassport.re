open Kanae;

module Done = {
  type t('ok, 'invalid, 'error) =
    | OK('ok)
    | Invalid('invalid)
    | Error('error);
};

module Strategy = {
  module Basic = {
    type t;
  };
  type t = Basic.t;
};

module LocalStrategy = {
  module Basic = {
    type invalid = {. "message": string};
    type authenticator('ok, 'invalid, 'error) =
      (
        string,
        string,
        (Js.Null.t('error), Js.False.t('ok), Js.Null.t('invalid)) => unit
      ) =>
      unit;
    [@bs.module "passport-local"] [@bs.new]
    external create : authenticator('ok, 'invalid, 'error) => Strategy.Basic.t =
      "Strategy";
  };
  type invalid = {message: string};
  type authenticator('ok, 'error) =
    (~username: string, ~password: string) => Done.t('ok, invalid, 'error);
  let basicInvalid = error => {"message": error.message};
  let create = (~f) => {
    let basicF = (username, password, done_) =>
      switch (f(~username, ~password)) {
      | Done.OK(value) =>
        done_(Js.Null.empty, Js.False.some(value), Js.Null.empty)
      | Invalid(error) =>
        done_(
          Js.Null.empty,
          Js.False.none(),
          Js.Null.return(basicInvalid(error))
        )
      | Error(error) =>
        done_(Js.Null.return(error), Js.False.none(), Js.Null.empty)
      };
    Basic.create(basicF);
  };
};

module Basic = {
  [@bs.module "passport"] external use : Strategy.Basic.t => unit = "use";
  [@bs.module "passport"]
  external initialize : unit => NozomiMiddleware.Basic.f = "initialize";
  [@bs.module "passport"]
  external session : unit => NozomiMiddleware.Basic.f = "session";
  [@bs.module "passport"]
  external authenticate : (string, Js.Any.t) => NozomiMiddleware.Basic.f =
    "authenticate";
};

let middleware = () => Basic.initialize() |> NozomiMiddleware.from;

let session = () => Basic.session() |> NozomiMiddleware.from;

let use = (strategy: Strategy.t) : unit => Basic.use(strategy);

/*
 let authenticate = (auth: Authenticator.t) : NozomiMiddleware.f => {
   let (name, opts) = Authenticator.basic(auth);
   Basic.authenticate(name, opts) |> NozomiMiddleware.from;
 };
 */
module Authenticator = {
  let local = (~failureFlash=false, ~successRedirect, ~failureRedirect) =>
    Basic.authenticate(
      "local",
      Js.Any.return({
        "successRedirect": successRedirect,
        "failureRedirect": failureRedirect,
        "failureFlash": Js.fromBool(failureFlash)
      })
    )
    |> NozomiMiddleware.from;
};
/*
 module Authenticator = {
   module Local = {
     type t = {
       successRedirect: string,
       failureRedirect: string,
       failureFlash: bool
     };
   };
   type t =
     | Local(Local.t);
   let basic = auth : (string, Js.Any.t) =>
     switch auth {
     | Local(opts) => (
         "local",
         Js.Any.return({
           "successRedirect": opts.successRedirect,
           "failureRedirect": opts.failureRedirect,
           "failureFlash": Js.fromBool(opts.failureFlash)
         })
       )
     };

 };


 */