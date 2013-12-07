package org.pinguinobutton;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.HashSet;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Lightweight TCP server that supports multiple clients connecting on a given port. 
 * 
 * @author Niels Brouwers
 *
 */
public class Server
{
	
	// Server socket for the TCP connection
	private ServerSocket serverSocket = null;
	
	// TCP port to use
	private final int port;

	// List of connected clients. Concurrency-safe arraylist because Clients can join/leave at any point,
	// which means inserts/removes can occur at any time from different threads.
	private CopyOnWriteArrayList<Client> clients = new CopyOnWriteArrayList<Client>();
	
	// Set of event listeners for this server
	private HashSet<ServerListener> listeners = new HashSet<ServerListener>();
	
	// Indicates that the main server loop should keep running. 
	private boolean keepAlive = true;
	
	// Main thread.
	private Thread listenThread;
	
	/**
	 * Constructs a new server instance on port 4567.
	 */
	public Server()
	{
		this(4567);
	}
	
	/**
	 * Constructs a new server instance.
	 * @param port TCP port to use.
	 */
	public Server(int port)
	{
		this.port = port;
	}

	/**
	 * @return TCP port this server accepts connections on.
	 */
	public int getPort()
	{
		return port;
	}

	/**
	 * @return true iff the server is running.
	 */
	public boolean isRunning()
	{
		return listenThread!=null && listenThread.isAlive();
	}
	
	/**
	 * @return the number of currently connected clients
	 */
	public int getClientCount()
	{
		return clients.size();
	}
	
	/**
	 * Starts the server.
	 * @throws IOException
	 */
	public void start() throws IOException
	{
		keepAlive = true;
		serverSocket = new ServerSocket(port);
		
		(listenThread = new Thread(){
			public void run()
			{
				Socket socket;
				try
				{
					while (keepAlive)
					{
						
						try {

							socket = serverSocket.accept();

							// Create Client object.
							Client client = new Client(Server.this, socket);
							clients.add(client);
							
							// Notify listeners.
							for (ServerListener listener : listeners)
								listener.onClientConnect(Server.this, client);
						
						} catch (SocketException ex)
						{
							// A SocketException is thrown when the stop method calls 'close' on the
							// serverSocket object. This means we should break out of the connection
							// accept loop.
							keepAlive = false;
						}
					
					}
					
				} catch (IOException e)
				{
					// TODO
				}
			}
		}).start();
		
		// Notify listeners.
		for (ServerListener listener : listeners)
			listener.onServerStarted(this);
		
	}
	
	/**
	 * Stops the server
	 */
	public void stop()
	{
		// Stop listening in the TCP port.
		if (serverSocket!=null)
			try
			{
				serverSocket.close();
			} catch (IOException e)
			{
				// TODO
			}
			
		// Close all clients.
		for (Client client : clients)
			client.close();
		
		// Notify listeners.
		for (ServerListener listener : listeners)
			listener.onServerStopped(this);
		
	}
	
	/**
	 * Called by the Client class to remove itself from the server. 
	 * 
	 * @param client Client to disconnect
	 */
	protected void disconnectClient(Client client)
	{
		this.clients.remove(client);
		
		for (ServerListener listener : listeners)
			listener.onClientDisconnect(Server.this, client);
	}

	/**
	 * Fires the receive event. Called by the client when it has new data to offer.
	 * 
	 * @param client source client
	 * @param data data 
	 */
	protected void receive(Client client, byte data[])
	{
		// Notify listeners.
		for (ServerListener listener : listeners)
			listener.onReceive(client, data);
	}
	
	/**
	 * Adds a server listener to the server
	 * @param listener a ServerListener instance 
	 */
	public void addListener(ServerListener listener)
	{
		this.listeners.add(listener);
	}
	
	/**
	 * Removes a server listener from the server
	 * @param listener a ServerListener instance 
	 */
	public void removeListener(ServerListener listener)
	{
		this.listeners.remove(listener);
	}
	
	/**
	 * Send bytes to all connected clients.
	 *  
	 * @param data data to send
	 * @throws IOException
	 */
	public void send(byte[] data) throws IOException
	{
		for (Client client : clients)
			client.send(data);
	}

	/**
	 * Send a string to all connected clients
	 * @param str string to send
	 * @throws IOException
	 */
	public void send(String str) throws IOException
	{
		for (Client client : clients)
			client.send(str);
	}

}
