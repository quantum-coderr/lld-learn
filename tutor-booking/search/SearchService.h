#pragma once

#include "../logger/Logger.h"
#include "../user/Tutor.h"
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// SearchService  –  Single Responsibility: search tutors by subject only
// Interface Segregation: exposes only the search contract needed by the system
// ─────────────────────────────────────────────────────────────────────────────
class SearchService {

public:
  std::vector<Tutor *> searchBySubject(std::vector<Tutor *> &tutors,
                                       std::string subject) {

    LOG.info("SearchService", "Searching for tutors with subject: \"" + subject + "\"");

    std::vector<Tutor *> result;

    for (auto tutor : tutors) {
      if (tutor->getSubject() == subject)
        result.push_back(tutor);
    }

    LOG.info("SearchService",
             "Found " + std::to_string(result.size()) + " tutor(s) for subject: \"" + subject + "\"");
    return result;
  }
};