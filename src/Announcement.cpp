//
// Created by gryffyn on 17/03/2020.
//

#include "Announcement.hpp"
#include "ObjStore.hpp"

Announcement::Announcement(ObjStore& u_db, std::string i_text, std::string i_title) {
    db = u_db;
    db.insert("INSERT INTO Announcements(ann_title, ann_text) VALUES ('" + i_title + "', '" + i_text + "');");
}