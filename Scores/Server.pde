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
  println("SENDING NAME");
  PostRequest startPost = new PostRequest(server_addr + "setname");
  startPost.addData("cname", commandName);
  try {
    startPost.send();
    String resp = startPost.getContent();
    println("SERVER SN UNSW:" + resp);
    while (!resp.equals("SERVER_SET_CMDNAME_OK"))
    {
      wait(1);
      startPost.send();
      resp = startPost.getContent();
      println("SERVER SN UNSW:" + resp);
    }
    STATE = PLAYING;
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
    println("SERVER SD UNSW:" + resp);
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
      // IDLE -> WAIT_NAME : to ZOMBIE_NAME
      // WAIT_START -> PLAYING : to show Scores with current Name
      // ? -> IDLE : load scores

      if (Response.equals("WAIT_NAME") && STATE != ZOMBIE_NAME) 
      {
        println("WAIT NAME START");
        STATE = ZOMBIE_NAME;
        INPUT_GAME = 0;
        enter_name[INPUT_GAME] = true;
        enter_time[INPUT_GAME] = false;
        for (int c = 0; c < 3; c++)
        {
          cmd_names[0][c] = '*';
          char_ix[0][c] = 0;
        }
      }
      if (Response.equals("PLAYING") && STATE == ZOMBIE_NAME) 
      {
        println("START PLAY");
        STATE = PLAYING;
        INPUT_GAME = 0;
        enter_name[INPUT_GAME] = false;
        for (int c = 0; c < 3; c++) 
        {
          cmd_names[0][c] = '*';
          char_ix[0][c] = 0;
        }
      }
      if (Response.equals("IDLE") && STATE == PLAYING)
      {
        println("STOP PLAY, UPDATE SCORES");
        score_loaded = false;
        STATE = SHOW_SCORES;
        for (int c = 0; c < 3; c++) 
        {
          cmd_names[0][c] = '*';
          char_ix[0][c] = 0;
        }
      }
      if (Response.equals("IDLE") && STATE != PLAYING)
      {
        // Reset Scores
        println("RESET SCORES");
        STATE = SHOW_SCORES;
        enter_name[INPUT_GAME] = false;
        enter_time[INPUT_GAME] = false;
        for (int c = 0; c < 3; c++) 
        {
          cmd_names[0][c] = '*';
          char_ix[0][c] = 0;
        }
      }
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
