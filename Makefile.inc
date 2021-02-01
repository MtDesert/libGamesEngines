ifeq ($(Game),true)
	name=Engines
else ifeq ($(Client),true)
	name=Client
else ifeq ($(Server),true)
	name=Server
else ifeq ($(Tools),true)
	name=Tools
endif
include ../libGames$(name)/Makefile