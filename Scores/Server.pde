void connectToServer()
{
  if (!server_connect)
  {
    GetRequest getConnect = new GetRequest(server_addr+"connect");
    getConnect.send();
    while (!server_connect)
    {
      String resp = getConnect.getContent();
      if (resp.equals("OK")) server_connect = true;
      else getConnect.send();
    }
    println("Server connected");
  } else println("Server connected");
}


void sendName(String commandName)
{
  PostRequest startPost = new PostRequest(server_addr + "sendname");
  startPost.addData("cname", commandName);
  try {
    startPost.send();
    String resp = startPost.getContent();
    println("SERVER UNSW:" + resp);
    while (!resp.equals("SERVER_SET_CMDNAME_OK"))
    {
      wait(1);
      startPost.send();
      resp = startPost.getContent();
      println("SERVER UNSW:" + resp);
    }
    STATE = SHOW_RECORD;
    for (int n = 0; n < 3; n++) cmd_name[n] = '*';
  } 
  catch (Exception e) {
    println(e);
  }
}

void updateState()
{
  if (millis() - lastUpdate > 1000)
  {
    GetRequest getState = new GetRequest(server_addr+"getstate");
    getState.send();
    String Response = getState.getContent();
    lastUpdate = millis();
    if (Response.equals("WAIT_NAME")) STATE = ENTER_NAME;
    else STATE = SHOW_RECORD;
  }
}
