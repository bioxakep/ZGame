void serverConnect()
{
  if (!server_connect)
  {
    GetRequest testGet = new GetRequest(server_addr + "rst");
    testGet.send();
    long start_connect = millis();
    while (!server_connect && millis() - start_connect < server_connect_timeout)
    {
      try
      {
        String resp = testGet.getContent();
        if (resp.equals("OK")) server_connect = true;
        else testGet.send();
      }
      catch(Exception e)
      {
        println(e);
      }
      wait(1);
    }
    if (server_connect) println("Server connected");
    else println("Server unreachable");
  } else println("Server allready connected");
}

void waitName()
{
  GetRequest getName = new GetRequest(server_addr + "getname");
  getName.send();
  try
  {
    String resp = getName.getContent();
    if (resp.equals("SERVER_WAIT_NAME"))
    {
      wait(2);
      println("SERVER UNSW:" + resp);
    } else command_name = resp;
  }
  catch(Exception e)
  {
    println(e);
  }
}

void sendStart()
{
  GetRequest startGet = new GetRequest(server_addr + "startgame");
  startGet.send();
  try {
    String resp = startGet.getContent();
    if (resp.equals("OK"))
    {
      wait(2);
      println("SERVER UNSW:" + resp);
    }
  }
  catch(Exception e)
  {
    println(e);
  }
}

void sendEnd()
{
  PostRequest endGet = new PostRequest(server_addr + "endgame");
  String gData = join(nf(gTimes, 0), ","); 
  endGet.addData("gdata", gData);
  endGet.addData("scores", str(currScores));
  endGet.send();
  try {
    String resp = endGet.getContent();
    if (resp.equals("OK"))
    {
      wait(2);
      println("SERVER UNSW:" + resp);
    }
  }
  catch(Exception e)
  {
    println(e);
  }
}
