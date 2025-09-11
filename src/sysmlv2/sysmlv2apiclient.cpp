#include "sysmlv2apiclient.hpp"

using namespace std;

json SysMLv2APIClient::getProjects(const int pageSize = 50) {
  string path = "/projects";
  if (pageSize > 0) {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getProjectById(const UUID& projectId) {
  string path = "/projects/" + projectId;
  return httpGet(path);
}

json SysMLv2APIClient::createProject(const std::string& name, const std::string& description) {
  json body = {{"name", name}};
  if (! description.empty()) {
    body["description"] = description;
  }
  return httpPost("/projects", body);
}

json SysMLv2APIClient::updateProject(const UUID& projectId,
  const std::string& name,
  const std::string& description) {
  json body = json::object();
  if (!name.empty()) {
    body["name"] = name;
  }
  if (!description.empty()) {
    body["description"] = description;
  }

  std::string path = "/projects/" + projectId;
  return httpPut("PUT", path, body);
}

json SysMLv2APIClient::deleteProject(const UUID& projectId) {
  const string path = "/projects/" + projectId;
  return httpDelete(path);
}

json SysMLv2APIClient::getElements(const UUID& projectId,
  const std::string &commitId, int pageSize = 50) {
  string path = "/projects/" + projectId + "/commits/" + commitId + "/elements";
  if (pageSize > 0) {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

json SysMLv2APIClient::getElementById(const UUID& projectId, const UUID& commitId,
  const UUID& elementId) {
  string path = "/projects/" + projectId + "/commits/" + commitId + "/elements/" + elementId;
  return httpGet(path);
}

// Get root elements
json SysMLv2APIClient::getRootElements(const UUID& projectId, const UUID& commitId,
  const int pageSize = 50) {
  std::string path = "/projects/" + projectId + "/commits/" + commitId + "/roots";
  if (pageSize > 0) {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

// Get relationships by related element
json SysMLv2APIClient::getRelationshipsByRelatedElement(const UUID& projectId,
  const UUID& commitId, const UUID& relatedElementId, const UUID& direction = "both") {
  std::string path = "/projects/" + projectId + "/commits/" + commitId +
    "/elements/" + relatedElementId + "/relationships";
  path += "?direction=" + direction;
  return httpGet(path);
}

// Get all commits
json SysMLv2APIClient::getCommits(const std::string &projectId, int pageSize = 50)
{
  std::string path = "/projects/" + projectId + "/commits";
  if (pageSize > 0)
  {
    path += "?page[size]=" + std::to_string(pageSize);
  }
  return httpGet(path);
}

// Get commit by ID
json SysMLv2APIClient::getCommitById(const std::string &projectId, const std::string &commitId)
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId;
  return httpGet(path);
}

// Get commit changes
json SysMLv2APIClient::getCommitChanges(const std::string &projectId, const std::string &commitId,
                      const std::vector<std::string> &changeTypes = {})
{
  std::string path = "/projects/" + projectId + "/commits/" + commitId + "/changes";

  if (!changeTypes.empty())
  {
    path += "?changeTypes=";
    for (size_t i = 0; i < changeTypes.size(); ++i)
    {
      if (i > 0)
        path += ",";
      path += changeTypes[i];
    }
  }

  return httpGet(path);
}

// Get branches
json SysMLv2APIClient::getBranches(const std::string &projectId)
{
  std::string path = "/projects/" + projectId + "/branches";
  return httpGet(path);
}

// Get branch by ID
json SysMLv2APIClient::getBranchById(const std::string &projectId, const std::string &branchId)
{
  std::string path = "/projects/" + projectId + "/branches/" + branchId;
  return httpGet(path);
}

// Create branch
json SysMLv2APIClient::createBranch(const std::string &projectId, const std::string &branchName,
                  const std::string &headCommitId)
{
  json body = {
      {"name", branchName},
      {"head", headCommitId}};

  std::string path = "/projects/" + projectId + "/branches";
  return httpPost(path, body);
}

// Delete branch
json SysMLv2APIClient::deleteBranch(const std::string &projectId, const std::string &branchId)
{
  std::string path = "/projects/" + projectId + "/branches/" + branchId;
  return httpDelete(path);
}

// Get tags
json SysMLv2APIClient::getTags(const std::string &projectId)
{
  std::string path = "/projects/" + projectId + "/tags";
  return httpGet(path);
}

// Get tag by ID
json SysMLv2APIClient::getTagById(const std::string &projectId, const std::string &tagId)
{
  std::string path = "/projects/" + projectId + "/tags/" + tagId;
  return httpGet(path);
}

// Create tag
json SysMLv2APIClient::createTag(const std::string &projectId, const std::string &tagName,
               const std::string &taggedCommitId)
{
  json body = {
      {"name", tagName},
      {"taggedCommit", taggedCommitId}};

  std::string path = "/projects/" + projectId + "/tags";
  return httpPost(path, body);
}

// Delete tag
json SysMLv2APIClient::deleteTag(const std::string &projectId, const std::string &tagId)
{
  std::string path = "/projects/" + projectId + "/tags/" + tagId;
  return httpDelete(path);
}

// Create commit
json SysMLv2APIClient::createCommit(const std::string &projectId, const json &changes,
                  const std::string &branchId = "", const std::string &description = "")
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

// Diff commits
json SysMLv2APIClient::diffCommits(const std::string &projectId, const std::string &baseCommitId,
                 const std::string &compareCommitId,
                 const std::vector<std::string> &changeTypes = {})
{
  std::string path = "/projects/" + projectId + "/commits/" + compareCommitId + "/diff";
  path += "?baseCommit=" + baseCommitId;

  if (!changeTypes.empty())
  {
    path += "&changeTypes=";
    for (size_t i = 0; i < changeTypes.size(); ++i)
    {
      if (i > 0)
        path += ",";
      path += changeTypes[i];
    }
  }

  return httpGet(path);
}

// Get queries
json SysMLv2APIClient::getQueries(const std::string &projectId)
{
  std::string path = "/projects/" + projectId + "/queries";
  return httpGet(path);
}

// Get query by ID
json SysMLv2APIClient::getQueryById(const std::string &projectId, const std::string &queryId)
{
  std::string path = "/projects/" + projectId + "/queries/" + queryId;
  return httpGet(path);
}

// Create query
json SysMLv2APIClient::createQuery(const std::string &projectId, const std::string &name,
                 const json &queryDefinition)
{
  json body = queryDefinition;
  body["name"] = name;

  std::string path = "/projects/" + projectId + "/queries";
  return httpPost(path, body);
}

// Execute query by ID
json SysMLv2APIClient::executeQueryById(const std::string &projectId, const std::string &queryId,
                      const std::string &commitId = "")
{
  std::string path = "/projects/" + projectId + "/queries/" + queryId + "/results";
  if (!commitId.empty())
  {
    path += "?commitId=" + commitId;
  }
  return httpGet(path);
}

// Execute ad-hoc query
json SysMLv2APIClient::executeQuery(const std::string &projectId, const json &query,
                  const std::string &commitId = "")
{
  std::string path = "/projects/" + projectId + "/query-results";
  if (!commitId.empty())
  {
    path += "?commitId=" + commitId;
  }
  return httpPost(path, query);
}