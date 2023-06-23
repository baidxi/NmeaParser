//
// Created by juno on 23-6-22.
//

#include <iostream>
#include <cstring>
#include <future>
#include <algorithm>
#include "Nmea.h"

using namespace std;

static void GNVTG_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GNGGA_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GNGLL_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GPGSA_Process(shared_ptr<string> &msg, Nmea *nmea);
static void BDGSA_Process(shared_ptr<string> &msg, Nmea *nmea);
static void BDGSV_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GPGSV_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GNZDA_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GPTXT_Process(shared_ptr<string> &msg, Nmea *nmea);
static void GNRMC_Process(shared_ptr<string> &msg, Nmea *nmea);

Nmea::Nmea() {
  processor_map.insert(make_pair("GNVTG", GNVTG_Process));
  processor_map.insert(make_pair("GNGGA", GNGGA_Process));
  processor_map.insert(make_pair("GNGLL", GNGLL_Process));
  processor_map.insert(make_pair("GPGSA", GPGSA_Process));
  processor_map.insert(make_pair("BDGSA", BDGSA_Process));
  processor_map.insert(make_pair("BDGSV", BDGSV_Process));
  processor_map.insert(make_pair("GPGSV", GPGSV_Process));
  processor_map.insert(make_pair("GNZDA", GNZDA_Process));
  processor_map.insert(make_pair("GPTXT", GPTXT_Process));
  processor_map.insert(make_pair("GNRMC", GNRMC_Process));

  data.insert(make_pair("VTG", malloc(sizeof(vtg))));
  data.insert(make_pair("GLL", malloc(sizeof(gll))));
  data.insert(make_pair("GSV", malloc(sizeof(gsv))));
  data.insert(make_pair("GGA", malloc(sizeof(gga))));
}

Nmea::~Nmea() {
  for_each(data.begin(), data.end(), [](const pair<string, void *> &ptr) {
	free(ptr.second);
  });
}

int Nmea::parse(std::shared_ptr<std::string> &msg) {
  auto type = msg->substr(1, 5);
  auto processor = processor_map.find(type);

  if (processor != processor_map.end()) {
	(void)async(launch::async, [&processor, &msg, this]() {
	  processor->second(msg, this);
	});
  } else {
	fprintf(stderr, "%s\n", type.c_str());
  }

  return 0;
}

void *Nmea::get(const std::string &name) {
  auto ret = data.find(name);

  if (ret != data.end())
	return ret->second;
  else
	return nullptr;
}

static vector<string>
split_string(const string &str) {
  vector<string> tokens;
  size_t start = 0;
  size_t end = str.find(',');

  while (end != string::npos) {
	if ((end - start) == 0)
	  tokens.emplace_back("0");
	else
	  tokens.emplace_back(str.substr(start, end - start));
	start = end + 1;
	end = str.find(',', start);
  }

  tokens.emplace_back(str.substr(start));

  return tokens;
}

static void GNVTG_Process(std::shared_ptr<std::string> &msg, Nmea *nmea) {
  vector<string> tokens = split_string(msg->c_str());

  vtg *v = (vtg *)nmea->get("VTG");

  v->deg1 = strtod(tokens[1].c_str(), nullptr);
  v->deg2 = strtod(tokens[3].c_str(), nullptr);
  v->speed = strtod(tokens[5].c_str(), nullptr);
  v->kph = strtod(tokens[8].c_str(), nullptr);
}

static void GNGGA_Process(shared_ptr<string> &msg, Nmea *nmea) {
  vector<string> tokens = split_string(msg->c_str());

  gga *v = (gga *)nmea->get("GGA");

  strncpy(v->time, tokens[1].c_str(), tokens[1].length());
  v->lat = strtod(tokens[2].c_str(), nullptr);
  v->ns = *tokens[3].c_str();
  v->lon = strtod(tokens[4].c_str(), nullptr);
  v->ew = *tokens[5].c_str();
  v->quality = strtol(tokens[6].c_str(), nullptr, 10);
  v->sat_num = (int)strtol(tokens[7].c_str(), nullptr, 10);
  v->horizontal = strtod(tokens[8].c_str(), nullptr);
  v->ant_alt = strtod(tokens[9].c_str(), nullptr);
  v->ant_units = *tokens[10].c_str();
  v->geoidal = strtod(tokens[11].c_str(), nullptr);
  v->geoiadl_units = *tokens[12].c_str();
  v->age = strtod(tokens[13].c_str(), nullptr);
  v->id = strtoul(tokens[14].c_str(), nullptr, 10);
}
static void GNGLL_Process(shared_ptr<string> &msg, Nmea *nmea) {
  vector<string> tokens = split_string(msg->c_str());

  gll *v = (gll *)nmea->get("GLL");

  v->lat = strtod(tokens[1].c_str(), nullptr);
  v->ns = *tokens[2].c_str();
  v->lon = strtod(tokens[3].c_str(), nullptr);
  v->ew = *tokens[4].c_str();
  strncpy(v->time, tokens[5].c_str(), tokens[5].length());
  v->status = *tokens[6].c_str();
}
static void GPGSA_Process(shared_ptr<string> &msg, Nmea *nmea) {

}
static void BDGSV_Process(shared_ptr<string> &msg, Nmea *nmea) {

}
static void BDGSA_Process(shared_ptr<string> &msg, Nmea *nmea) {

}
static void GPGSV_Process(shared_ptr<string> &msg, Nmea *nmea) {
  auto tokens = split_string(msg->c_str());
  int index = 0;

  int group_count = (int)strtol(tokens[1].c_str(), nullptr, 10);
  int group_index = (int)strtol(tokens[2].c_str(), nullptr, 10);
  int sat_count = (int)strtol(tokens[3].c_str(), nullptr, 10);

  int loop_count;
  gsv *v = (gsv *)nmea->get("GSV");

  v->sat_count = sat_count;
  v->group_count = group_count;

  if ((group_count - group_index) == 0)
	loop_count = v->sat_count % group_count;
  else
	loop_count = 4;

  gsv_group *group = v->group + (group_index - 1);
  group->num_info = loop_count;

  for (int i = 0; i < loop_count; ++i) {
	index = 4 * (i + 1);
	group->infos[i].sat_num = strtol(tokens[index + 0].c_str(), nullptr, 10);
	group->infos[i].deg1 = strtol(tokens[index + 1].c_str(), nullptr, 10);
	group->infos[i].deg2 = strtol(tokens[index + 2].c_str(), nullptr, 10);
	group->infos[i].snr = strtol(tokens[index + 3].c_str(), nullptr, 10);
  }
}
static void GNZDA_Process(shared_ptr<string> &msg, Nmea *nmea) {

}
static void GPTXT_Process(shared_ptr<string> &msg, Nmea *nmea) {

}

static void GNRMC_Process(shared_ptr<string> &msg, Nmea *nmea) {

}
