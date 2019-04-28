void drawFrames()
{
  fill(0);
  rect(0, 0, width-2, height-2);
  line(game_width, 0, game_width, height);
  line(2*game_width, 0, 2*game_width, height);
}

void drawHeaders()
{
  image(z_logo, game_width/2 - 180, game_width/2 - 180, 360, 100);
  image(r_logo, game_width + game_width/2 - 180, game_width/2 - 180, 360, 100);
  image(g_logo, 2*game_width + game_width/2 - 180, game_width/2 - 180, 360, 100);
  fill(255);
  textSize(text_size);
  for (int i = 0; i < 3; i++) 
  {
    textAlign(LEFT);
    text(rank, i*game_width + game_width/2 - 180, game_width/2 - 180 + 100 + header_size + 5);
    textAlign(CENTER);
    text(time, i*game_width + game_width/2, game_width/2 - 180 + 100 + header_size + 5);
    textAlign(RIGHT);
    text(team, i*game_width + game_width/2 + 180, game_width/2 - 180 + 100 + header_size + 5);
  }
}

void drawScores()
{
  for (int g = 0; g < 3; g++)
  {
    if (!score_loaded)
    {
      try
      {
        gameData[g] = loadJSONArray(server_addr+"getscores?game=" + gameNames[g]);
        println("Game Data from " + gameNames[g] + " recieved: " + str(gameData[g].size())+ " commands");
      }
      catch (Exception e)
      {
        println(e);
        println("Game Data from " + gameNames[g] + " not recieved");
      }
    }
    for (int c = 0; c < gameData[g].size() && c < 5; c++)
    {
      JSONObject co = gameData[g].getJSONObject(c);
      String name = co.getString("Name");
      Integer time = co.getInt("Time");
      textAlign(CENTER);
      textSize(text_size);
      text(str(c+1), g*game_width + game_width/2 - 180 + rank_width/2, game_width/2 - 180 + 100 + header_size + 5 + (c+1)*text_size);
      String timestr = getTime(hours(Integer.valueOf(time)), minutes(Integer.valueOf(time)), seconds(Integer.valueOf(time)));
      text(timestr, g*game_width + game_width/2, game_width/2 - 180 + 100 + header_size + 5 + (c+1)*text_size);
      text(name, g*game_width + game_width/2 + 180 - team_width/2, game_width/2 - 180 + 100 + header_size + 5 + (c+1)*text_size);
    }
  }
  score_loaded = true;
}

void enterName(int game)
{
  textAlign(CENTER);
  float cmd_name_offset = 0.0;
  boolean canSend = true;
  String cmd_name_str = "";
  for (int i = 0; i < 3; i++)
  {
    textAlign(LEFT);
    if (cmd_name[i] == '*' ) 
    {
      if(millis() - start_draw_error_rect < 2000) 
      {
        fill(red);
        rect(game_width/2 + game*game_width + 180 - team_width/2 - 1.5*textWidth(cmd_name[i]) + cmd_name_offset, game_width/2 - 100 + header_size + (min(5,gameData[game].size())+1)*text_size - 3, textWidth(cmd_name[i]), text_size/2);
      }
      canSend = false;
    }
    if (char_pos == i && enter_name)
    {
      fill(200);
      strokeWeight(0);
      rect(game_width/2 + game*game_width + 180 - team_width/2 - 1.5*textWidth(cmd_name[i]) + cmd_name_offset, game_width/2 - 100 + header_size + (min(5,gameData[game].size())+1)*text_size - 3, textWidth(cmd_name[i]), text_size/2);
      fill(0);
    } else fill(255);
    text(cmd_name[i], game_width/2 + game*game_width + 180 - team_width/2 - 1.5*textWidth(cmd_name[i]) + cmd_name_offset, game_width/2 - 180 + 100 + header_size + 5 + (min(5,gameData[game].size())+1)*text_size);
    cmd_name_offset += textWidth(cmd_name[i]);
    cmd_name_str += cmd_name[i];
  }
  cmd_name_width = cmd_name_offset;
  name_ok = canSend;
  //println("name_OK:" + str(name_ok));
}

void enterTime(int game)
{
  String timestr = getTime(hours(Integer.valueOf(cmd_time)), minutes(Integer.valueOf(cmd_time)), seconds(Integer.valueOf(cmd_time)));
  fill(255);
  textAlign(CENTER);
  text(timestr, game*game_width + game_width/2, game_width/2 - 180 + 100 + header_size + 5 + (min(5,gameData[game].size())+1)*text_size);
}

String getTime(int h, int m, int s)
{
  String h_str = str(h);
  if (h < 10) h_str = "0" + str(h);
  String m_str = str(m);
  if (m < 10) m_str = "0" + str(m);
  String s_str = str(s);
  if (s < 10) s_str = "0" + str(s);
  return h_str+":"+m_str+":"+s_str;
}

int seconds(int secs) {
  return int(secs % 60);
}
int minutes(int secs) {
  return int((secs / (60)) % 60);
}
int hours(int secs) {
  return int((secs / (60*60)) % 24);
}
