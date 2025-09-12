#pragma once

#include "../httptoolclient.hpp"
#include "../mcptoolregistry.hpp"

using Identifier = std::string;

/// @brief Client for OMG Systems Modeling API and Services
///
/// A tool client that complies with the <a href="https://www.omg.org/spec/SystemsModelingAPI">OMG
/// Systems Modeling Application Programming Interface (API) and Services</a> 1.0 specification.
/// This class is a specialization of HttpToolClient and extends it with HTTP/REST calls for the
/// SysML v2 API for accessing SysML v2 model repositories.
class SysMLv2APIClient : public HttpToolClient {
public:
  explicit SysMLv2APIClient(MCPToolRegistry& mcpToolRegistry);

#pragma region Project Service Operations
  /// @brief Get all projects.
  /// @param pageSize
  /// @return
  json getProjects(const int pageSize = 50);

  /// @brief Get project with the given id (projectId).
  /// @param projectId is the UUID assigned to the project.
  /// @return
  json getProjectById(const Identifier& projectId);

  /// @brief Create a new project with the given name and description (optional).
  /// @param name is the project's name.
  /// @param description is an optional statement that provides details about the project.
  /// @return 
  json createProject(const std::string &name, const std::string &description = "");

  /// @brief Update the project with the given id (projectId).
  /// @param projectId is the UUID assigned to the project.
  /// @param name 
  /// @param description 
  /// @return 
  json updateProject(const Identifier& projectId, const std::string &name = "",
                     const std::string &description = "");

  /// @brief Delete the project with the given id (projectId)
  /// @param projectId is the UUID assigned to the project.
  /// @return 
  json deleteProject(const Identifier& projectId);
#pragma endregion

#pragma region Element Navigation Service Operations
  // Get all elements in a project at specific commit
  json getElements(const Identifier& projectId, const Identifier& commitId, int pageSize = 50);

  // Get element by ID
  json getElementById(const Identifier& projectId, const Identifier& commitId, const Identifier& elementId);

  // Get root elements
  json getRootElements(const Identifier& projectId, const Identifier& commitId, const int pageSize = 50);

  // Get relationships by related element
  json getRelationshipsByRelatedElement(const Identifier& projectId, const Identifier& commitId,
    const Identifier& relatedElementId, const std::string& direction = "both");
#pragma endregion

#pragma region Project Data Versioning Service Operations
  // Get all commits
  json getCommits(const Identifier &projectId, int pageSize = 50);

  // Get commit by ID
  json getCommitById(const Identifier &projectId, const Identifier &commitId);

  // Get commit changes
  json getCommitChanges(const Identifier &projectId, const Identifier &commitId,
    const std::vector<std::string> &changeTypes = {});

  // Get branches
  json getBranches(const Identifier &projectId);

  // Get branch by ID
  json getBranchById(const Identifier &projectId, const Identifier &branchId);

  // Create branch
  json createBranch(const Identifier &projectId, const Identifier &branchName,
    const Identifier &headCommitId);

  // Delete branch
  json deleteBranch(const Identifier &projectId, const Identifier &branchId);

  // Get tags
  json getTags(const Identifier &projectId);

  // Get tag by ID
  json getTagById(const Identifier &projectId, const Identifier &tagId);

  // Create tag
  json createTag(const Identifier &projectId, const Identifier &tagName,
    const Identifier &taggedCommitId);

  // Delete tag
  json deleteTag(const Identifier &projectId, const Identifier &tagId);

  // Create commit
  json createCommit(const Identifier &projectId, const json &changes,
    const Identifier &branchId = "", const std::string &description = "");

  // Diff commits
  json diffCommits(const Identifier &projectId, const Identifier &baseCommitId,
    const Identifier &compareCommitId, const std::vector<std::string>& changeTypes = {});
#pragma endregion

#pragma region Query Service Operations
  // Get queries
  json getQueries(const Identifier &projectId);

  // Get query by ID
  json getQueryById(const Identifier &projectId, const Identifier &queryId);

  // Create query
  json createQuery(const Identifier &projectId, const std::string &name,
    const json &queryDefinition);

  // Execute query by ID
  json executeQueryById(const Identifier &projectId, const Identifier &queryId,
    const Identifier &commitId = "");

  // Execute ad-hoc query
  json executeQuery(const Identifier &projectId, const json& query,
    const Identifier& commitId = "");
#pragma endregion
  
  SysMLv2APIClient() = delete;

private:
  void setupSysMLv2APITools(MCPToolRegistry& mcpToolRegistry);
};