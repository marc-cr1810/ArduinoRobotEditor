#pragma once

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#define YAML_BEGIN()				YAML::Emitter out;\
									out << YAML::BeginMap

#define YAML_END()					out << YAML::EndMap;\
									std::ofstream fout(filepath);\
									fout << out.c_str();\
									fout.close()

#define YAML_KEY(key)				out << YAML::Key << key
#define YAML_KEY_VALUE(key, value)	out << YAML::Key << key << YAML::Value << value
#define YAML_BEGIN_SEQ(key)			out << YAML::Key << key << YAML::Value << YAML::BeginSeq
#define YAML_END_SEQ()				out << YAML::EndSeq
#define YAML_BEGIN_MAP()			out << YAML::BeginMap
#define YAML_END_MAP()				out << YAML::EndMap

static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}