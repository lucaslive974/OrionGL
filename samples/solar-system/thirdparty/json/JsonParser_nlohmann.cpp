//
// Created by lucas.lima on 30/11/2025.
//

#include <Constants.h>
#include "JsonParser.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

namespace oriongl::samples::solar_system {
    void from_json(const json &j, solar_system::CorpData &c) {
        c.name = j.value("name", "unnamed");
        c.rotationScaler = j.value("rotation_speed", 1.0f);
        c.translationScaler = j.value("translation_speed", 1.0f);
        c.radius = j.value("radius", 1.0f);

        if (j.contains("defines") && j["defines"].is_array())
            c.defines = j["defines"].get<std::vector<std::string>>();

        if (j.contains("textures") && j["textures"].is_array()) {
            for (const auto &tex : j["textures"]) {
                c.textures.push_back(utils::constants::ASSETS_PATH + tex.get<std::string>());
            }
        }

        if (c.textures.size() == 1) {
            c.textures.push_back(utils::constants::TEXTURE_BLACK_FALLBACK);
        }

        if (j.contains("position") && j.at("position").size() == 3) {
            c.pos = j.at("position").get<std::array<float, 3>>();
        } else {
            c.pos = {0.0f, 0.0f, 0.0f};
        }
    }
}

namespace oriongl::samples::utils {
    JsonParser::system_data JsonParser::readSystemData() {
        planets_data planets;
        stars_data stars;

        std::ifstream input_file{constants::CORPS_DATA, std::ios::in};
        if (!input_file.is_open()) {
            throw std::runtime_error("Could not open solar system data file: " + constants::CORPS_DATA);
        }

        json parsed_json = json::parse(input_file);

        for (auto &planet_json: parsed_json.at("planets")) {
            planets.push_back(planet_json.get<solar_system::CorpData>());
        }

        for (auto &star_json: parsed_json.at("stars")) {
            stars.push_back(star_json.get<solar_system::CorpData>());
        }

        return std::make_pair(std::move(planets), std::move(stars));
    };
}

