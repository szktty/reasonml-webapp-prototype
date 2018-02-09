open Kanae;

open Nozomi;

/* Pug template module */
module View =
  Nozomi.View.Make(
    {
      /* template variables */
      /* WARN: this type is 'a of Js.t('a), do not use {. "...": ...} format */
      type locals = {
        .
        title: string,
        name: string
      };
    }
  );

/* template file */
let view = View.create("index");

/* get "/" */
let handler = (~request, ~response, ~next) => {
  View.render(
    view,
    ~response,
    ~locals={"title": "Welcome!", "name": "Alice"},
    ~f=(~error, ~html) =>
    Response.send(response, html)
  );
  next();
};