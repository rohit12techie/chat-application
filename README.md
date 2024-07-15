# cpp-chat-application

# to register client.
{
    "register": {
        "client_id": "client1",
    }
}

# Broker Client to Broker Server when ever Broker Client become available
{
	"ping" : {
		"client_id": "client1",
	}
}
# Response from Server the connection is established.
{
	"ping" : {
		"response": "pong",
	}
}

# Sending text message to another client.
{
    "message": {
        "from": "client1",
		"to": "client2",
		"text": "Hello",
    }
}
