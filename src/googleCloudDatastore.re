type key;

type boxedInt;
type boxedDouble;
type coordinates;

type id = int;
type name = string;
type propertyName = string;
type kind = string;
type namespace = string;
type path 'a = Js.t {. namespace : namespace, path : (kind, 'a)};
type cursor = string;
type consistency_ = string;

type consistency =
  | Strong
  | Eventual;

type moreResults =
  | AfterLimit
  | AfterCursor
  | NoMore;

type runInfo = {
  endCursor: cursor,
  moreResults: moreResults
};
let parseRun cb => (fun error result info => {
  cb error result {
    endCursor: info##endCursor,
    moreResults: (switch info##moreResults {
      | "MORE_RESULTS_AFTER_LIMIT" => AfterLimit
      | "MORE_RESULTS_AFTER_CURSOR" => AfterCursor
      | "NO_MORE_RESULTS" => NoMore
      | _ => NoMore
    })
  };
});

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
  external filter : propertyName => Js.Json.t => t = "" [@@bs.send.pipe: t];
  external filterWithOperator : propertyName => string => Js.Json.t => t =
    "filter" [@@bs.send.pipe: t];

  /** [order query]  Sort the results by a property name in ascending or
      descending order. By default, an ascending sort order will be used. **/
  external order : propertyName => t = "" [@@bs.send.pipe: t];
  external orderWithDirection : propertyName => Js.t {. descending: Js.boolean}
    => t = "order" [@@bs.send.pipe: t];

  /** [limit query] Set a limit on a query. **/
  external limit : int => t = "" [@@bs.send.pipe: t];

  /** [hasAncestor query] Filter a query by ancestors. **/
  external hasAncestor : key => t = "" [@@bs.send.pipe: t];

  /** [endCursor query] Set an ending cursor to a query. This function is
      normally called `end` but it was renamed since there is a name conflict
      with Reason. **/
  external endCursor : cursor => t = "end" [@@bs.send.pipe: t];

  /** [startCursor query] Set a starting cursor to a query. Was renamed to be
      consistent with `endCursor`. **/
  external startCursor : cursor => t = "start" [@@bs.send.pipe: t];

  /** [groupBy query] Group query results by a list of properties. **/
  external groupBy : array propertyName => t = "" [@@bs.send.pipe: t];

  /** [offset query] Set an offset on a query. **/
  external offset : int => t = "" [@@bs.send.pipe: t];

  /** [select query] Retrieve only select properties from the matched entities.
      **/
  external select : propertyName => t = "" [@@bs.send.pipe: t];
  external selectMultiple : array propertyName => t = "" [@@bs.send.pipe: t];

  /** [run query] Run the query. **/
  external run_ : t => (
    Js.nullable Error.t =>
    /** results **/
    array (Js.t {..}) =>
    /** info **/
    Js.t {. endCursor: cursor, moreResults: string }
    => unit
  ) => unit = "" [@@bs.send];
  let run t cb => {
    run_ t (parseRun cb);
  };

  external runWithConsistency_ : t => consistency_ => (
    Js.nullable Error.t =>
    /** results **/
    array (Js.t {..}) =>
    /** info **/
    Js.t {. endCursor: cursor, moreResults: string }
    => unit
  ) => unit = "run" [@@bs.send];
  let runWithConsistency t consistency cb => {
    runWithConsistency_ t (switch consistency {
      | Strong => "strong"
      | Eventual => "eventual"
    }) cb
  };

  /** Returns a promise **/
  external runPromise : t => Js.Promise.t (array (Js.t {..})) =
    "run" [@@bs.send];
  external runPromiseWithConsistency : t => consistency_ =>
    Js.Promise.t (array (Js.t {..})) = "run" [@@bs.send];
};

module Datastore = {
  type t;

  type modificationCallback = (Js.nullable Error.t => Js.Json.t => unit);

  /** Helper functions to get Datastore's objects. **/
  external int : t => string => boxedInt = "" [@@base.send];
  external double : t => float => boxedDouble = "" [@@base.send];

  /** [make ()] creates an instance of the datastore class. */
  external make : unit => t = "@google-cloud/datastore" [@@bs.module];

  /** [key datastore] Helpers to create a Key object. **/
  /** This function creates an incomplete key. **/
  external keyByKind : t => kind => key = "key" [@@bs.send];
  external keyByID : t => (kind, id) => key = "key" [@@bs.send];
  external keyByBoxedID : t => (kind, boxedInt) => key = "key" [@@bs.send];
  external keyByName : t => (kind, name) => key = "key" [@@bs.send];
  external keyByPathAndID : t => path id => key = "key" [@@bs.send];
  external keyByPathAndBoxedID : t => path boxedInt => key = "key" [@@bs.send];
  external keyByPathAndName : t => path name => key = "key" [@@bs.send];

  /** [allocateIds] Generate IDs without creating entities. The key needs to be
      incomplete. **/
  external allocateIds : t => key => int =>
    /** error, result, complete API response **/
    (Js.nullable Error.t => array key => Js.t {..} => unit) => unit =
    "" [@@bs.send];

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
    modificationCallback => unit = "" [@@bs.send];
  external saveMultiple : t => array (Js.t {. key : key, data : Js.t {..}}) =>
    modificationCallback => unit = "save" [@@bs.send];

  /** [update datastore] **/
  external update : t => Js.t {. key : key, data : Js.t {..}} =>
    modificationCallback => unit = "" [@@bs.send];
  external updateMultiple : t => array (Js.t {. key : key, data : Js.t {..}}) =>
    modificationCallback => unit = "update" [@@bs.send];

  /** [upsert datastore] **/
  external upsert : t => Js.t {. key : key, data : Js.t {..}} =>
    modificationCallback => unit = "" [@@bs.send];
  external upsertMultiple : t => array (Js.t {. key : key, data : Js.t {..}}) =>
    modificationCallback => unit = "upsert" [@@bs.send];

  /** [delete datastore] Delete all entities identified with the specified
      key(s). **/
  external delete : t => key => modificationCallback => unit = "" [@@bs.send];
  external deleteMultiple : t => array key => modificationCallback => unit =
    "delete" [@@bs.send];

  /** [createQuery datastore] Create a query for the specified kind **/
  external createQuery : t => kind => Query.t = "" [@@bs.send];
  external createQueryWithNamespace : t => namespace => kind => Query.t =
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
    Js.t {. endCursor: cursor, moreResults: string }
    => unit
  ) => unit = "" [@@bs.send];
  external runQueryWithConsistency : t => consistency_ => Query.t => (
    Js.nullable Error.t =>
    /** results **/
    array (Js.t {..}) =>
    /** info **/
    Js.t {. endCursor: cursor, moreResults: string }
    => unit
  ) => unit = "run" [@@bs.send];
  /** Returns a promise **/
  external runQueryPromise : t => Query.t => Js.Promise.t (array (Js.t {..})) =
    "runQuery" [@@bs.send];
  external runQueryPromiseWithConsistency : t => consistency_ => Query.t =>
    Js.Promise.t (array (Js.t {..})) = "runQuery" [@@bs.send];

  /** [geoPoint datastore] Helper function to get a Datastore Geo Point object.
      **/
  external geoPoint : t => Js.t {. latitude: float, longitude: float} =>
    coordinates = "" [@@bs.send];
};
