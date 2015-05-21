package org.pinguinobutton;

public interface ServerListener
{

	/**
	 * Called when the server is started.
	 * @param server the server that is started 
	 */
	public void onServerStarted(Server server);

	/**
	 * Called when the server is stopped.
	 * @param server the server that is stopped 
	 */
	public void onServerStopped(Server server);
	
	/**
	 * Called when a new client (device) connects to the server.
	 * @param server the server that is started 
	 * @param client the Client object representing the newly connected client
	 */
	public void onClientConnect(Server server, Client client);
	
	/**
	 * Called when a new client (device) disconnects from the server.
	 * @param server the server that is started 
	 * @param client the Client that disconnected
	 */
	public void onClientDisconnect(Server server, Client client);

	/**
	 * Called when data is received from the client.
	 * @param client source client
	 * @param data data
	 */
	public void onReceive(Client client, byte data[]);

}
