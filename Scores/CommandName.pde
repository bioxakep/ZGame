int char_pos = 0;
char[] cmd_name;
int[] char_ix;

void keyPressed()
{
  //println(keyCode);
  // 37 - left, 38 - up, 39 - right, 40 - down
  if (STATE == ENTER_NAME)
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
      if(can_send) sendName(toStr(cmd_name));
      else println("Wrong name");
    }
  }
}

void printName()
{
  float offX = 10+textWidth("NAME: ");
  textAlign(LEFT);
  text("NAME: ", 10, height/2);
  float passWidth = 0.0;
  float tri_hei = 2;
  float char_h = 20;
  boolean canSend = true;
  for (int i = 0; i < 3; i++)
  {
    text(cmd_name[i], offX + passWidth, height/2);
    if (cmd_name[i] == '*') canSend = false;
    if (char_pos == i)
    {
      line(offX + passWidth, height/2 - char_h, offX + passWidth + textWidth(cmd_name[i])/2, height/2 - char_h - tri_hei);
      line(offX + passWidth + textWidth(cmd_name[i])/2, height/2 - char_h - tri_hei, offX + passWidth + textWidth(cmd_name[i]), height/2 - char_h);
      line(offX + passWidth, height/2 + 3, offX + passWidth + textWidth(cmd_name[i])/2, height/2 + 5);
      line(offX + passWidth + textWidth(cmd_name[i])/2, height/2 + 5, offX + passWidth + textWidth(cmd_name[i]), height/2 + 3);
    }
    passWidth += textWidth(cmd_name[i]);
  }
  can_send = canSend;
}
