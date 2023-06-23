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

Nmea::Nmea()
{
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

	data.insert(make_pair("VTG", malloc(sizeof (vtg))));
	data.insert(make_pair("GLL", malloc(sizeof (gll))));

}

Nmea::~Nmea() {
	for_each(data.begin(),data.end(), [](const pair<string, void *> &ptr){
	  free(ptr.second);
	});
}

int Nmea::parse(std::shared_ptr<std::string> &msg)
{
	auto type = msg->substr(1, 5);
	auto processor = processor_map.find(type);

	if (processor != processor_map.end()) {
		(void)async(launch::async, [&processor, &msg, this](){
			processor->second(msg, this);
		});
	} else {
		fprintf(stderr,"%s\n", type.c_str());
	}

	return 0;
}

void *Nmea::get_data(const std::string &name)
{
	auto ret = data.find(name);

	if (ret != data.end())
		return ret->second;
	else
		return nullptr;
}

static vector<string>
split_string(const string &str)
{
	vector<string> tokens;
	size_t start = 0;
	size_t end = str.find(',');

	while(end != string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(',', start);
	}

	tokens.push_back(str.substr(start));

	return tokens;
}

static void GNVTG_Process(std::shared_ptr<std::string> &msg, Nmea *nmea)
{
	vector<string> tokens = split_string(msg->c_str());
	vtg *v = (vtg *)nmea->get_data("VTG");

	v->deg1 = strtod(tokens[1].c_str(), nullptr);
	v->deg2 = strtod(tokens[3].c_str(), nullptr);
	v->speed = strtod(tokens[5].c_str(), nullptr);
	v->kph = strtod(tokens[8].c_str(), nullptr);
}

static void GNGGA_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void GNGLL_Process(shared_ptr<string> &msg, Nmea *nmea)
{
	vector<string> tokens = split_string(msg->c_str());
	gll *v = (gll *)nmea->get_data("GLL");

	v->lat = strtod(tokens[1].c_str(), nullptr);
	v->ns = *tokens[2].c_str();
	v->lon = strtod(tokens[3].c_str(), nullptr);
	v->ew = *tokens[4].c_str();
	strncpy(v->time, tokens[5].c_str(), tokens[5].length());
	v->status = *tokens[6].c_str();
}
static void GPGSA_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void BDGSV_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void BDGSA_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void GPGSV_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void GNZDA_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
static void GPTXT_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}

static void GNRMC_Process(shared_ptr<string> &msg, Nmea *nmea)
{

}
