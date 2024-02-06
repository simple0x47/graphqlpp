# Commits

## Prefix

Every commit must contain one of the following prefixes:

* `+` Feature or code addition, i.e. `+ serial processing`.
* `-` Feature or code removal, i.e. `- useless using`.
* `r` Refactor, i.e. `r renamed 'Supplier' to 'Provider'`.
* `d` Documentation addition, i.e. `d public manager's API`.
* `t` Test addition, i.e. `t GraphQL query parser`.
* `f` Incorrect behaviour fix, i.e. `f incorrect calculation`.

There are cases when a commit contains a combination of the previous prefixes,
in that case, the developer must select the prefix which matches with most of the
commit's parts.
For example, lets say there is a commit with 40% code addition,
30% code removal and 30% documentation addition. In this case the dominating prefix
would be `+`.

## Message

The message must explain what has been done in **50 characters**. If there's the need
to further explain the change, a new line must be inserted in order to separate the
commit's summary from the body. *The body has no character limits*.

For example:

```
- boost dependency
Due to the size of the dependency, we have chosen to restrict the usage of boost.
```