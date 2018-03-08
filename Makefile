edit:echoServer.o 
	g++ -o edit  echoServer.o 
client:echoClient.o 
	cc -o client  echoClient.o 
echoServer.o:src/echoServer.c    
	g++ -c src/echoServer.c
echoClient.o:src/echoClient.c    
	cc -c src/echoClient.c
clean :
	rm edit echoServer.o
