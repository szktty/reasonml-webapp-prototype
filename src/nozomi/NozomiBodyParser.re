open Kanae;

module Basic = {
  [@bs.module "body-parser"] [@bs.val]
  external json : unit => NozomiMiddleware.Basic.t = "json";
  [@bs.module "body-parser"] [@bs.val]
  external raw : unit => NozomiMiddleware.Basic.t = "raw";
  [@bs.module "body-parser"] [@bs.val]
  external text : unit => NozomiMiddleware.Basic.t = "text";
  [@bs.module "body-parser"] [@bs.val]
  external urlencoded : unit => NozomiMiddleware.Basic.t = "urlencoded";
};

module URLEncodedOptions = {
  module Basic = {
    type t = {
      .
      "extended": Js.boolean,
      "inflate": Js.boolean
    };
  };
  type t = {
    extended: bool,
    inflate: bool
  };
  let basic = opts => {
    "extended": Js.fromBool(opts.extended),
    "inflate": Js.fromBool(opts.inflate)
  };
};

let json = () => NozomiMiddleware.from(Basic.json());

let raw = () => NozomiMiddleware.from(Basic.raw());

let text = () => NozomiMiddleware.from(Basic.text());

let urlencoded = () => NozomiMiddleware.from(Basic.urlencoded());