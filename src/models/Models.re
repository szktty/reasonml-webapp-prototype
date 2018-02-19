open Kanae.Base;

module User = {
  open Siika;
  include
    Record.Make(
      {
        include Record.Generic;
        let init = seq =>
          define(
            seq,
            Model.Spec.(
              model(
                "user",
                ~options=Model.Options.create(~tableName="user", ())
              )
              +> string("username", ~allowNull=true)
              +> string("password", ~allowNull=true)
            )
          );
      }
    );
  let username = getter("username", Getter.string);
  let setUsername = setter("username", Setter.string);
};

let init = seq => User.init(seq);
/*Siika.sync(seq) |> then_(_seq => resolve(Js.log("sync ok"))) |> ignore;*/