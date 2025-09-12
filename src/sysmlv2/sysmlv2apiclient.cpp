#include "sysmlv2apiclient.hpp"

using namespace std;

SysMLv2APIClient::SysMLv2APIClient(MCPToolRegistry &mcpToolRegistry) {
  setupSysMLv2APITools(mcpToolRegistry);
}

json SysMLv2APIClient::getProjects(const int pageSize) {
  string path = "/projects";
  if (pageSize > 0) {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getProjectById(const Identifier &projectId) {
  string path = "/projects/" + projectId;
  return httpGet(path);
}

json SysMLv2APIClient::createProject(const std::string &name, const std::string &description)
{
  json body = {{"name", name}};
  if (!description.empty())
  {
    body["description"] = description;
  }
  return httpPost("/projects", body);
}

json SysMLv2APIClient::updateProject(const Identifier &projectId,
                                     const std::string &name,
                                     const std::string &description)
{
  json body = json::object();
  if (!name.empty())
  {
    body["name"] = name;
  }
  if (!description.empty())
  {
    body["description"] = description;
  }

  std::string path = "/projects/" + projectId;
  return httpPut("PUT", path, body);
}

json SysMLv2APIClient::deleteProject(const Identifier &projectId)
{
  const string path = "/projects/" + projectId;
  return httpDelete(path);
}

json SysMLv2APIClient::getElements(const Identifier &projectId,
                                   const std::string &commitId, int pageSize)
{
  string path = "/projects/" + projectId + "/commits/" + commitId + "/elements";
  if (pageSize > 0)
  {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getElementById(const Identifier &projectId, const Identifier &commitId,
                                      const Identifier &elementId)
{
  string path = "/projects/" + projectId + "/commits/" + commitId + "/elements/" + elementId;
  return httpGet(path);
}

json SysMLv2APIClient::getRootElements(const Identifier &projectId, const Identifier &commitId,
                                       const int pageSize)
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId + "/roots";
  if (pageSize > 0)
  {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getRelationshipsByRelatedElement(const Identifier &projectId,
                                                        const Identifier &commitId, const Identifier &relatedElementId, const Identifier &direction)
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId +
                     "/elements/" + relatedElementId + "/relationships";
  path += "?direction=" + direction;
  return httpGet(path);
}

json SysMLv2APIClient::getCommits(const Identifier &projectId, const int pageSize)
{
  std::string path = "/projects/" + projectId + "/commits";
  if (pageSize > 0)
  {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getCommitById(const Identifier &projectId, const Identifier &commitId)
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId;
  return httpGet(path);
}

json SysMLv2APIClient::getCommitChanges(const Identifier &projectId,
                                        const Identifier &commitId, const std::vector<std::string> &changeTypes)
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId + "/changes";

  if (!changeTypes.empty())
  {
    path += "?changeTypes=";
    for (size_t index = 0; index < changeTypes.size(); ++index)
    {
      if (index > 0)
        path += ",";
      path += changeTypes[index];
    }
  }

  return httpGet(path);
}

json SysMLv2APIClient::getBranches(const Identifier &projectId)
{
  std::string path = "/projects/" + projectId + "/branches";
  return httpGet(path);
}

json SysMLv2APIClient::getBranchById(const Identifier &projectId, const Identifier &branchId)
{
  std::string path = "/projects/" + projectId + "/branches/" + branchId;
  return httpGet(path);
}

// Create branch
json SysMLv2APIClient::createBranch(const Identifier &projectId,
                                    const std::string &branchName, const Identifier &headCommitId)
{
  json body = {
      {"name", branchName},
      {"head", headCommitId}};

  const std::string path = "/projects/" + projectId + "/branches";
  return httpPost(path, body);
}

// Delete branch
json SysMLv2APIClient::deleteBranch(const Identifier &projectId, const Identifier &branchId)
{
  const std::string path = "/projects/" + projectId + "/branches/" + branchId;
  return httpDelete(path);
}

// Get tags
json SysMLv2APIClient::getTags(const Identifier &projectId)
{
  const std::string path = "/projects/" + projectId + "/tags";
  return httpGet(path);
}

// Get tag by ID
json SysMLv2APIClient::getTagById(const Identifier &projectId, const Identifier &tagId)
{
  std::string path = "/projects/" + projectId + "/tags/" + tagId;
  return httpGet(path);
}

// Create tag
json SysMLv2APIClient::createTag(const Identifier &projectId, const std::string &tagName,
                                 const Identifier &taggedCommitId)
{
  const json body = {
      {"name", tagName},
      {"taggedCommit", taggedCommitId}};

  const std::string path = "/projects/" + projectId + "/tags";
  return httpPost(path, body);
}

json SysMLv2APIClient::deleteTag(const Identifier &projectId, const Identifier &tagId)
{
  const std::string path = "/projects/" + projectId + "/tags/" + tagId;
  return httpDelete(path);
}

// Create commit
json SysMLv2APIClient::createCommit(const Identifier &projectId, const json &changes,
                                    const Identifier &branchId, const std::string &description)
{
  json body = {
      {"change", changes}};

  if (!description.empty())
  {
    body["description"] = description;
  }

  std::string path = "/projects/" + projectId + "/commit";
  if (!branchId.empty())
  {
    path += "?branchId=" + branchId;
  }

  return httpPost(path, body);
}

json SysMLv2APIClient::diffCommits(const Identifier &projectId, const Identifier &baseCommitId,
                                   const Identifier &compareCommitId, const std::vector<std::string> &changeTypes)
{
  std::string path = "/projects/" + projectId + "/commits/" + compareCommitId + "/diff";
  path += "?baseCommit=" + baseCommitId;

  if (!changeTypes.empty())
  {
    path += "&changeTypes=";
    for (size_t index = 0; index < changeTypes.size(); ++index)
    {
      if (index > 0)
        path += ",";
      path += changeTypes[index];
    }
  }

  return httpGet(path);
}

json SysMLv2APIClient::getQueries(const Identifier &projectId)
{
  std::string path = "/projects/" + projectId + "/queries";
  return httpGet(path);
}

json SysMLv2APIClient::getQueryById(const Identifier &projectId, const Identifier &queryId)
{
  std::string path = "/projects/" + projectId + "/queries/" + queryId;
  return httpGet(path);
}

json SysMLv2APIClient::createQuery(const Identifier &projectId, const std::string &name,
                                   const json &queryDefinition)
{
  json body = queryDefinition;
  body["name"] = name;

  std::string path = "/projects/" + projectId + "/queries";
  return httpPost(path, body);
}

json SysMLv2APIClient::executeQueryById(const Identifier &projectId, const Identifier &queryId,
                                        const Identifier &commitId)
{
  std::string path = "/projects/" + projectId + "/queries/" + queryId + "/results";
  if (!commitId.empty())
  {
    path += "?commitId=" + commitId;
  }
  return httpGet(path);
}

json SysMLv2APIClient::executeQuery(const Identifier &projectId, const json &query,
                                    const Identifier &commitId)
{
  std::string path = "/projects/" + projectId + "/query-results";
  if (!commitId.empty())
  {
    path += "?commitId=" + commitId;
  }
  return httpPost(path, query);
}

void SysMLv2APIClient::setupSysMLv2APITools(MCPToolRegistry &mcpToolRegistry)
{
  mcpToolRegistry.registerTool(
      "sysml_list_projects",
      "List all available SysML v2 projects.",
      {{"type", "object"},
       {"properties", {{"pageSize", {{"type", "integer"}, {"description", "Maximum number of projects per page"}, {"default", 50}}}}}},
      [this](const json &params) -> json
      {
        try
        {
          int pageSize = params.value("pageSize", 50);
          json result = getProjects(pageSize);

          return {
              {"content", {{{"type", "text"}, {"text", "Projects:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_project",
      "Get specific SysML v2 project by ID.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}}},
       {"required", {"projectId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          json result = getProjectById(projectId);

          return {
              {"content", {{{"type", "text"}, {"text", "Project Details:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_create_project",
      "Create new SysML v2 project",
      {{"type", "object"},
       {"properties", {{"name", {{"type", "string"}, {"description", "Name of the project"}}}, {"description", {{"type", "string"}, {"description", "Optional description of the project"}}}}},
       {"required", {"name"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string name = params["name"];
          std::string description = params.value("description", "");
          json result = createProject(name, description);

          return {
              {"content", {{{"type", "text"}, {"text", "Created Project:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_elements",
      "Get all elements in a project at specific commit.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"commitId", {{"type", "string"}, {"description", "UUID of the commit"}}}, {"pageSize", {{"type", "integer"}, {"description", "Maximum number of elements per page"}, {"default", 50}}}}},
       {"required", {"projectId", "commitId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          std::string commitId = params["commitId"];
          int pageSize = params.value("pageSize", 50);

          json result = getElements(projectId, commitId, pageSize);

          return {
              {"content", {{{"type", "text"}, {"text", "Elements:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_element",
      "Get specific element by ID.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"commitId", {{"type", "string"}, {"description", "UUID of the commit"}}}, {"elementId", {{"type", "string"}, {"description", "UUID of the element"}}}}},
       {"required", {"projectId", "commitId", "elementId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          std::string commitId = params["commitId"];
          std::string elementId = params["elementId"];

          json result = getElementById(projectId, commitId, elementId);

          return {
              {"content", {{{"type", "text"}, {"text", "Element Details:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_root_elements",
      "Get root elements in a project.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"commitId", {{"type", "string"}, {"description", "UUID of the commit"}}}}},
       {"required", {"projectId", "commitId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          std::string commitId = params["commitId"];

          json result = getRootElements(projectId, commitId);

          return {
              {"content", {{{"type", "text"}, {"text", "Root Elements:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_branches",
      "Get all branches in a project.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}}},
       {"required", {"projectId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          json result = getBranches(projectId);

          return {
              {"content", {{{"type", "text"}, {"text", "Branches:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_get_commits",
      "Get commit history for a project.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"pageSize", {{"type", "integer"}, {"description", "Maximum number of commits per page"}, {"default", 50}}}}},
       {"required", {"projectId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          int pageSize = params.value("pageSize", 50);
          json result = getCommits(projectId, pageSize);

          return {
              {"content", {{{"type", "text"}, {"text", "Commits:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });


  mcpToolRegistry.registerTool(
      "sysml_execute_query",
      "Execute SysML v2 query on project data.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"query", {{"type", "object"}, {"description", "Query definition with select, where, orderBy clauses"}}}, {"commitId", {{"type", "string"}, {"description", "Optional commit ID to query against"}}}}},
       {"required", {"projectId", "query"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          json query = params["query"];
          std::string commitId = params.value("commitId", "");

          json result = executeQuery(projectId, query, commitId);

          return {
              {"content", {{{"type", "text"}, {"text", "Query Results:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });

  mcpToolRegistry.registerTool(
      "sysml_diff_commits",
      "Compare differences between two commits.",
      {{"type", "object"},
       {"properties", {{"projectId", {{"type", "string"}, {"description", "UUID of the project"}}}, {"baseCommitId", {{"type", "string"}, {"description", "UUID of the base commit"}}}, {"compareCommitId", {{"type", "string"}, {"description", "UUID of the compare commit"}}}, {"changeTypes", {{"type", "array"}, {"items", {"type", "string", "enum", {"CREATED", "UPDATED", "DELETED"}}}, {"description", "Filter by change types"}}}}},
       {"required", {"projectId", "baseCommitId", "compareCommitId"}}},
      [this](const json &params) -> json
      {
        try
        {
          std::string projectId = params["projectId"];
          std::string baseCommitId = params["baseCommitId"];
          std::string compareCommitId = params["compareCommitId"];

          std::vector<std::string> changeTypes;
          if (params.contains("changeTypes"))
          {
            for (const auto &ct : params["changeTypes"])
            {
              changeTypes.push_back(ct.get<std::string>());
            }
          }

          json result = diffCommits(projectId, baseCommitId, compareCommitId, changeTypes);

          return {
              {"content", {{{"type", "text"}, {"text", "Commit Differences:\n" + result["data"].dump(2)}}}}};
        }
        catch (const std::exception &e)
        {
          return {
              {"content", {{{"type", "text"}, {"text", "Error: " + std::string(e.what())}}}}};
        }
      });
}