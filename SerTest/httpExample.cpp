#include <iostream>
#include <boost/asio.hpp>
#include <httplib.h>
#include "Deserializer.h"
#include "furniture.h"
#include <boost/json.hpp>
#include "JSONDe.h"
#include "JSONSer.h"
#include "access.h"
#include "SerializeExt.h"
#include "DeserializeExt.h"
#include "httpExample.h"

boost::asio::io_service io_service;
boost::posix_time::seconds interval(10); // 10 seconds
boost::asio::deadline_timer timer(io_service, interval);


void task(const boost::system::error_code&)
{
	// do something
	updateCache();

	// reschedule the timer for the next interval
	timer.expires_at(timer.expires_at() + interval);
	timer.async_wait(task);
}



void runHttpExample()
{
	// load configuration for the server
	Configuration config(loadConfig());

	
	loadTestData();

	std::thread run_thread([&] {
		timer.async_wait(task);
		io_service.run();
		});




	httplib::Server svr;

	svr.Get(R"(/furniture/(.+))", [](const httplib::Request& req, httplib::Response& res) {
		auto name = req.matches[1];
		DeserializeBuffer<Furniture*> furniture;
		try {
			auto furniture = getFurniture(name);
			std::ostringstream stream;
			JSONSer ser(stream);
			write(furniture)(ser);

			res.set_content(req.body, "application/json"); 
			res.body = stream.str();

		}
		catch (...) {
		}

		});

	svr.Put("/furniture", [](const httplib::Request& req, httplib::Response& res) {

		DeserializeBuffer<Furniture*> furniture;
		try {
			boost::json::value v(boost::json::parse(req.body));
			JSONDe de(v);
			DeserialzationScope scope;
			deserialize(furniture.buffer())(de, scope);
			insertFurniture(**furniture);
			delete* furniture;
		}
		catch (...) {

		}

		});

	std::cout << "Port for server is: " << config.port() << std::endl;
	svr.listen("localhost", config.port());


}

