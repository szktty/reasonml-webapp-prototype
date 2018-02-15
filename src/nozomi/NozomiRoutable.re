type method_ =
  | Get
  | Post;

module type Router = {
  type router;
  let all: (router, string, NozomiMiddleware.f) => unit;
  let get: (router, string, NozomiMiddleware.f) => unit;
  let post: (router, string, NozomiMiddleware.f) => unit;
  /*
   let params: t => unit;
   let use: (t, string, NozomiMiddleware.f) => unit;
   */
};

module type S = {
  type router;
  let all: (router, string, NozomiMiddleware.f) => unit;
};

module Make = (R: Router) : (S with type router := R.router) => {
  let all = R.all;
  let get = R.get;
  let post = R.post;
};