open Kanae.Base;

module Basic = {
  type t;
  [@bs.get] external body : t => Js.Undefined.t(Js.Dict.t(Js.Any.t)) = "body";
  [@bs.get] external method_ : t => string = "method";
  [@bs.get] external originalURL : t => string = "originalUrl";
  [@bs.get] external path : t => string = "path";
  [@bs.get] external protocol : t => string = "protocol";
  [@bs.get] external query : t => Js.Json.t = "query";
  [@bs.get] external secure : t => Js.Boolean.t = "secure";
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

let basic = (req: t) : Basic.t => req;

let body = (req, ~key) : option(Js.Any.t) =>
  Basic.body(req)
  |> Js.Undefined.toOption
  |> Option.findMap(~f=dict => {
       Js.log("hello");
       Js.log(Basic.body(req));
       Js.log(dict);
       Js.Dict.get(dict, key);
     });

let stringBody = (req, ~key) =>
  body(req, key) |> Option.findMap(~f=Js.Any.string);

let method = Basic.method;

let isSecure = req => Basic.secure(req) |> Js.toBool;

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
  switch (String.split(path, ~on='.')) {
  | [] => None
  | comps => query0(comps, Basic.query(req)) |> Js.Json.decodeString
  };
};