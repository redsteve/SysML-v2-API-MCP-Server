#pragma once

#include "../httptoolclient.hpp"

using UUID = std::string;

/// @brief Client for OMG Systems Modeling API and Services
///
/// A tool client that complies with the <a href="https://www.omg.org/spec/SystemsModelingAPI">OMG
/// Systems Modeling Application Programming Interface (API) and Services</a> 1.0 specification.
/// This class is a specialization of HttpToolClient and extends it with HTTP/REST calls for the
/// SysML v2 API for accessing SysML v2 model repositories.
class SysMLv2APIClient : public HttpToolClient {
public:
#pragma region Project Service Operations
  /// @brief Get all projects.
  /// @param pageSize
  /// @return
  json getProjects(const int pageSize = 50);

  /// @brief Get project with the given id (projectId).
  /// @param projectId is the UUID assigned to the project.
  /// @return
  json getProjectById(const UUID& projectId);

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
  json updateProject(const UUID& projectId, const std::string &name = "",
                     const std::string &description = "");

  /// @brief Delete the project with the given id (projectId)
  /// @param projectId is the UUID assigned to the project.
  /// @return 
  json deleteProject(const UUID& projectId);
#pragma endregion
#pragma region Element Navigation Service Operations

  // Get all elements in a project at specific commit
  json getElements(const std::string &projectId, const std::string &commitId, int pageSize = 50);

  // Get element by ID
  json getElementById(const UUID& projectId, const UUID& commitId, const UUID& elementId);

  // Get root elements
  json getRootElements(const UUID& projectId, const UUID& commitId, const int pageSize = 50);

  // Get relationships by related element
  json getRelationshipsByRelatedElement(const UUID& projectId, const UUID& commitId,
    const UUID& relatedElementId, const std::string& direction = "both");
#pragma endregion
#pragma region Project Data Versioning Service Operations
  // Get all commits
  json getCommits(const UUID &projectId, int pageSize = 50);

  // Get commit by ID
  json getCommitById(const UUID &projectId, const UUID &commitId);

  // Get commit changes
  json getCommitChanges(const UUID &projectId, const UUID &commitId,
    const std::vector<std::string> &changeTypes = {});

  // Get branches
  json getBranches(const UUID &projectId);

  // Get branch by ID
  json getBranchById(const UUID &projectId, const UUID &branchId);

  // Create branch
  json createBranch(const UUID &projectId, const UUID &branchName,
    const UUID &headCommitId);

  // Delete branch
  json deleteBranch(const UUID &projectId, const UUID &branchId);

  // Get tags
  json getTags(const UUID &projectId);

  // Get tag by ID
  json getTagById(const UUID &projectId, const UUID &tagId);

  // Create tag
  json createTag(const UUID &projectId, const UUID &tagName,
    const UUID &taggedCommitId);

  // Delete tag
  json deleteTag(const UUID &projectId, const UUID &tagId);

  // Create commit
  json createCommit(const UUID &projectId, const json &changes,
    const UUID &branchId = "", const std::string &description = "");

  // Diff commits
  json diffCommits(const UUID &projectId, const UUID &baseCommitId,
    const UUID &compareCommitId, const std::vector<std::string>& changeTypes = {});
#pragma endregion
#pragma region Query Service Operations
  // Get queries
  json getQueries(const UUID &projectId);

  // Get query by ID
  json getQueryById(const UUID &projectId, const UUID &queryId);

  // Create query
  json createQuery(const UUID &projectId, const std::string &name,
    const json &queryDefinition);

  // Execute query by ID
  json executeQueryById(const UUID &projectId, const UUID &queryId,
    const UUID &commitId = "");

  // Execute ad-hoc query
  json executeQuery(const UUID &projectId, const json& query,
    const UUID& commitId = "");
};