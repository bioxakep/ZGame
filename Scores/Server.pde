void connectToServer()
{
  if (!server_connect)
  {
    GetRequest getConnect = new GetRequest(server_addr+"connect");
    try {
      getConnect.send();
    }
    catch(Exception e)
    {
      println(e);
    }
    while (!server_connect)
    {
      try {
        String resp = getConnect.getContent();
        if (resp.equals("OK")) server_connect = true;
        else getConnect.send();
      }
      catch(Exception e)
      {
        println(e);
      }
      wait(1);
    }
    println("Server connected");
  } else println("Server connected");
}


void sendName(String commandName)
{
  PostRequest startPost = new PostRequest(server_addr + "setname");
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
    for (int c = 0; c < 3; c++) 
    {
      cmd_name[c] = '*';
      char_ix[c] = 0;
    }
  } 
  catch (Exception e) {
    println(e);
  }
}


void sendData(String cName, int cTime)
{
  PostRequest dataPost = new PostRequest(server_addr + "setdata");
  String gnum = str(INPUT_GAME);
  String ctime = str(cTime);
  dataPost.addData("gnum", gnum);
  dataPost.addData("cname", cName);
  println("CMD_NAME_ADD:" + cName);
  dataPost.addData("ctime", ctime);
  try {
    dataPost.send();
    String resp = dataPost.getContent();
    println("SERVER UNSW:" + resp);
    while (!resp.equals("SERVER_ADD_DATA_OK"))
    {
      wait(1);
      dataPost.addData("gnum", gnum);
      dataPost.addData("cname", cName);
      dataPost.addData("ctime", ctime);
      dataPost.send();
      resp = dataPost.getContent();
      println("SERVER UNSW:" + resp);
    }
    STATE = SHOW_RECORD;
  }
  catch (Exception e) {
    println(e);
  }
}

void updateState()
{
  if (millis() - last_update_time > 1000)
  {
    GetRequest getState = new GetRequest(server_addr+"getstate");
    getState.send();
    try {
      String Response = getState.getContent();
      last_update_time = millis();
      if (Response.equals("WAIT_NAME")) 
      {
        STATE = ENTER_NAME;
        INPUT_GAME = 0;
        enter_name = true;
        for (int c = 0; c < 3; c++) cmd_name[c] = '*';
      } else STATE = SHOW_RECORD;
    }
    catch (Exception e) {
      println(e);
    }
  }
}

void wait(int secs)
{
  long t = millis();
  while (millis() - t < secs * 1000) {
    ;
  }
}
