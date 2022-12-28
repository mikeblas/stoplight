#include "CivetServer.h"

class ExampleHandler : public CivetHandler
{
public:
	bool handleGet(CivetServer* /* server */, struct mg_connection* conn)
	{
		mg_printf(conn,
			"HTTP/1.1 200 OK\r\nContent-Type: "
			"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>\r\n");
		mg_printf(conn,
			"<h2>This is an example text from a C++ handler</h2>\r\n");
		mg_printf(conn,
			"<p>To see a page from the A handler <a "
			"href=\"a\">click here</a></p>\r\n");
		mg_printf(conn,
			"<form action=\"a\" method=\"get\">"
			"To see a page from the A handler with a parameter "
			"<input type=\"submit\" value=\"click here\" "
			"name=\"param\" \\> (GET)</form>\r\n");
		mg_printf(conn,
			"<form action=\"a\" method=\"post\">"
			"To see a page from the A handler with a parameter "
			"<input type=\"submit\" value=\"click here\" "
			"name=\"param\" \\> (POST)</form>\r\n");
		mg_printf(conn,
			"<p>To see a page from the A/B handler <a "
			"href=\"a/b\">click here</a></p>\r\n");
		mg_printf(conn,
			"<p>To see a page from the *.foo handler <a "
			"href=\"xy.foo\">click here</a></p>\r\n");
		mg_printf(conn,
			"<p>To see a page from the WebSocket handler <a "
			"href=\"ws\">click here</a></p>\r\n");
		mg_printf(conn,
			"<p>To exit <a href=\"%s\">click here</a></p>\r\n",
			"EXIT_URI");
		mg_printf(conn, "</body></html>\r\n");
		return true;
	}
};

