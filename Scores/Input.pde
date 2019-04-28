int char_pos = 0;
char[] cmd_name;
int[] char_ix;

void keyPressed()
{
  println(keyCode);
  // 37 - left, 38 - up, 39 - right, 40 - down
  if (keyCode == 9 && STATE == SHOW_RECORD) // TAB
  {
    INPUT_GAME = (INPUT_GAME + 1) % 3;
    println("INPUT_GAME:" + str(INPUT_GAME));
    enter_name = true;
    enter_time = false;
    for (int c = 0; c < 3; c++) 
    {
      cmd_name[c] = '*';
      char_ix[c] = 0;
    }
    cmd_time = 0;
    char_pos = 0;
  }
  if (enter_time)
  {
    if (keyCode == 39) cmd_time = cmd_time + 10;
    if (keyCode == 37) cmd_time = max(0, cmd_time - 10);
    if (keyCode == 38) cmd_time = cmd_time + 1;
    if (keyCode == 40) cmd_time = max(cmd_time - 1, 0);
    if (keyCode == 10)
    {
      String cmd_name_str = toStr(cmd_name);
      sendData(cmd_name_str, cmd_time);
      enter_time = false;
      cmd_time = 0;
      for (int c = 0; c < 3; c++) 
      {
        cmd_name[c] = '*';
        char_ix[c] = 0;
      }
      score_loaded = false;
      INPUT_GAME = 0;
    }
  }
  if (enter_name)
  {
    if (keyCode == 37) char_pos = max(char_pos-1, 0);
    if (keyCode == 39) char_pos = min(char_pos+1, 2);
    if (keyCode == 40)
    {
      char_ix[char_pos] = max(0, char_ix[char_pos]-1);
      cmd_name[char_pos] = alpha.charAt(char_ix[char_pos]);
    }
    if (keyCode == 38) 
    {
      char_ix[char_pos] = min(alpha.length()-1, char_ix[char_pos]+1);
      cmd_name[char_pos] = alpha.charAt(char_ix[char_pos]);
    }
    if (keyCode == 10)
    {
      if (name_ok) 
      {
        if (STATE != ENTER_NAME) enter_time = true;
        else sendName(toStr(cmd_name));
        name_ok = false;
        enter_name = false;
        wait(1);
      } else 
      {
        start_draw_error_rect = millis();
        println("Wrong name, to quit press TAB");
      }
    }
  }
}
