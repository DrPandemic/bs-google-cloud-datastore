type key;

type kind 'a = (string, 'a);

type path 'a = Js.t {. namespace : string, path : kind 'a};

type cursor = string;

/** From bs-express */
module Error = {
  type t;
  /** Error type */

  external message : t => option string = ""
    [@@bs.send][@@bs.return null_undefined_to_opt];
  external name : t => option string = ""
    [@@bs.send][@@bs.return null_undefined_to_opt];
};

module Query = {
  type t;

  /** [limit query] Datastore allows querying on properties. Supported comparison
      operators are =, <, >, <=, and >=. "Not equal" and IN operators are
      currently not supported. **/
  /** Defaults to = **/
  external filter : string => Js.Json.t => t = "" [@@bs.send.pipe: t];
  external filterWithOperator : string => string => Js.Json.t => t =
    "filter" [@@bs.send.pipe: t];

  /** [order query]  Sort the results by a property name in ascending or
      descending order. By default, an ascending sort order will be used. **/
  external order : string => t = "" [@@bs.send.pipe: t];
  external orderWithDirection : string => Js.t {. descending: Js.boolean} => t
    = "order" [@@bs.send.pipe: t];

  /** [limit query] Set a limit on a query. **/
  external limit : int => t = "" [@@bs.send.pipe: t];

  /** [hasAncestor query] Filter a query by ancestors. **/
  external hasAncestor : key => t = "" [@@bs.send.pipe: t];
};

module Datastore = {
  type t;

  /** [make ()] creates an instance of the datastore class. */
  external make : unit => t = "@google-cloud/datastore" [@@bs.module];

  /** [key datastore] helpers to create a Key object **/
  external keyByKind : t => string => key = "key" [@@bs.send];
  /** (kind, id) **/
  external keyByID : t => (string, int) => key = "key" [@@bs.send];
  /** (kind, name) **/
  external keyByName : t => (string, string) => key = "key" [@@bs.send];
  external keyByPathAndID : t => path int => key = "key" [@@bs.send];
  external keyByPathAndName : t => path string => key = "key" [@@bs.send];

  /** [get datastore] retrieve the entities identified with the specified key(s)
      in the current transaction. **/
  external get : t => key => (Js.nullable Error.t => array (Js.t {..}) => unit) =>
    unit = "" [@@bs.send];
  external getMultiple :
    t => array key => (Js.nullable Error.t => array (Js.t {..}) => unit) => unit =
    "get" [@@bs.send];

  /** [save datastore] Insert or update the specified object(s). If a key is
      incomplete, its associated object is inserted and the original Key object
      is updated to contain the generated ID. **/
  external save : t => Js.t {. key : key, data : Js.t {..}} =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "save" [@@bs.send];
  external saveMultiple : t => array (Js.t {. key : key, data : Js.t {..}}) =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "save" [@@bs.send];

  /** [delete datastore] Delete all entities identified with the specified
      key(s). **/
  external delete : t => key => (Js.nullable Error.t => Js.Json.t => unit) => unit =
    "" [@@bs.send];
  external deleteMultiple : t => array key =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "delete" [@@bs.send];

  /** [createQuery datastore] Create a query for the specified kind **/
  /** kind **/
  external createQuery : t => string => Query.t = "" [@@bs.send];
  /** namespace => kind **/
  external createQueryWithNamespace : t => string => string => Query.t =
    "createQuery" [@@bs.send];

  /** [runQuery datastore] The query is run, and the results are returned as the
      second argument to your callback. A third argument may also exist, which is
      a query object that uses the end cursor from the previous query as the
      starting cursor for the next query. **/
  external runQuery : t => Query.t => (
      Js.nullable Error.t =>
      /** results **/
      array (Js.t {..}) =>
      /** info **/
      Js.t {. endCursor: cursor, moreResult: string }
      => unit
    ) => unit = "" [@@bs.send];
};
