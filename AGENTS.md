# CRY Agent Notes

- Use the `mcpcc` MCP server for code-context lookups when analyzing,
  reviewing, refactoring, or implementing changes in this repository.
- Pass `project: "cry-engine"` to `mcpcc` tools unless a user explicitly asks
  for another project.
- Before the first analytical `mcpcc` tool call in a session, check local edits
  with `git status --porcelain`. If edited files matter to the query, attach a
  local overlay with `mcp__mcpcc__attach_local_overlay` before relying on graph
  results.
- After editing files during a session, refresh the overlay for those files
  before the next analytical `mcpcc` query.
