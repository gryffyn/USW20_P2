//
// Created by gryffyn on 17/03/2020.
//

#include "Announcement.hpp"
#include "ObjStore.hpp"

Announcement::Announcement(ObjStore& u_db, const int& id, std::string i_text, std::string i_title) {
    db = u_db;
    std::stringstream ss;
    ss << "INSERT INTO Announcements(ann_author, ann_title, ann_text) VALUES (" << id << ", '"
       << i_title << "', '" << i_text << "');";
    db.insert(ss.str());
}