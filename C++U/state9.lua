local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local State = require("state")
local CommonStates = require("commonstates")
local State9 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={}, allowDispatch=true,}

local SlimeInterval = 2
local Twitter = 0 
local TwitterCounter = 0
local TwitterMax = 1
local AllowTwitter = false
local FrameRandom = nil
--玩家钢蛋
State9.Player.Routine = State:new()
State9.Player.Routine.execute = function(player)
  -- getStage():setCameraSlide(player.ref:getPosition(),CameraView.horizontal)
  if (Role[RoleIndex.wall].isDisposed) then
    player.ref:checkBorder(Vec2(2,ScreenHeight/3.8),Vec2(ScreenWidth,ScreenHeight/2))
  else
    player.ref:checkBorder(Vec2(2,ScreenHeight/3.8),Vec2(ScreenWidth-150,ScreenHeight/2))
  end
  PlayerStates.Routine.execute(player)
  if player.isBlocked ~= nil  then
    Role[1].ref:showHints(Hints.showJoystick)
  end
end 

State9.Npc.Wangba = State:new()
State9.Npc.Wangba.enter = function(npc)

  Twitter = 0
  TwitterCounter = 0 
  AllowTwitter = false
  FrameRandom = 
  {
    { frame = "agnail_red1.png", framePrefix="agnail_red", frameIndexes=nil,},
    { frame = "agnail_green1.png", framePrefix="agnail_green", frameIndexes=nil,},
    { frame = "agnail_blue1.png", framePrefix="agnail_blue", frameIndexes=nil,}, 
    { frame = "crab_red.png", framePrefix="", frameIndexes=nil,}, 
    { frame = "crab_green.png", framePrefix="", frameIndexes=nil,}, 
    { frame = "crab_blue.png", framePrefix="", frameIndexes=nil,}, 
  }
  Resh.playMusic(Music.AJourneyAwaits)
end
local counter = 1
State9.Npc.Wangba.execute = function(npc) 
  --  Luah.output("[State9.Npc.Wangba.execute]"..counter)
  local delay = 5
  if ( not State9.allowDispatch or Role[RoleIndex.wall].isDisposed or SlimeCounter >= SlimeMax ) then
    return
  end
  --使其不能部署
  State9.allowDispatch = false
  local rand = math.random(3) 
  local randFrame = FrameRandom[math.random(1,#FrameRandom)]
  local frame="crab_red.png"
  local frameIndexes=""
  local birdFramePrefix = ""
  local birdFrame = ""
  local birdStep =  Vec2(0.5*rand,0.5*rand)
  local birdHP = rand
  local pos = Vec2(17,7)
  local birdPos = Vec2(0,4)
  local offset =Vec2(0,0) 
  local zorder = 0

  local face = MovingDirection.toLeft
  if (rand == 1 ) then
    zorder = 6
    pos = Vec2(14,7)
    birdPos = Vec2(0,rand+2)
    face = MovingDirection.toRight
    birdFramePrefix = "bird_blue"
  elseif (rand ==2) then
    pos = Vec2(14,8)
    birdPos = Vec2(18,rand+2)
    zorder = 7
    face = MovingDirection.toLeft
    birdFramePrefix = "bird_blue"
  elseif (rand ==3) then
    pos = Vec2(14,9)
    birdPos = Vec2(0,rand+2)
    offset=Vec2(0,10)
    zorder = 8
    face = MovingDirection.toRight
    birdFramePrefix = "bird_blue"
  end

  local slime = { type=RoleType.npc, pos=pos,frame=randFrame.frame, framePrefix=randFrame.framePrefix, frameIndexes=randFrame. frameIndexes,offset=offset, step=SlimeStep, onCheckTile=false, isSolid=false,zorder=zorder, hp=1, lineIndex=rand,}
  slime.fsm = Fsm:new(slime,nil,State9.Npc.Slime)
  createRole(slime)
  SlimeCounter = SlimeCounter + 1

  --twitter 鸟
  if (AllowTwitter and Twitter < TwitterMax) then
    local bird = {type = RoleType.npc, frame="bird_gray1.png",step=birdStep, direction=face,framePrefix="bird_gray",pos =birdPos,  offset=Vec2(0,0), hp=birdHP, isSolid=false, isHarmful=false,framePrefix=birdFramePrefix, frameIndexes=FrameTemplate.bird}
    bird.fsm = Fsm:new(bird, CommonStates.Respawn,State9.Npc.Twitter)
    createRole(bird)
    Twitter = Twitter + 1
  end
  npc.fsm:changeState(State9.Npc.Command)
end 
State9.Npc.Wangba.onMessage = function(npc, telegram)
  --子弹
  if (telegram.message == MessageType.gotShot ) then
    npc.ref:setAnimation(Animate.getToastHurt("HP-250"))
    local status = npc.ref:gotHurt(250,Sound.wangbaHurt)
    Joystick.ref:setString(CommandPad.wall,tostring(Hints.barDick..npc.ref.hp))
    if (status == BodyStatus.dead) then
      Resh.playSound(Sound.manDied)
      Ref.stage:prompt(PopupInfo.passed())
    end
  end
end

State9.Dialogue.Start = State:new()
State9.Dialogue.Start.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State9.Dialogue.Start.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then
    Joystick.ref:setVisible(CommandPad.pad1,true)
    Joystick.ref:setVisible(CommandPad.pad2,true)
    Joystick.ref:setVisible(CommandPad.pad3,true) 
    Joystick.ref:setVisible(CommandPad.fire,true) 
    Role[1].fsm:changeState(State9.Npc.Command) 
    Role[RoleIndex.wangba].fsm:changeGlobal(State9.Npc.Wangba)
    Role[RoleIndex.wangba].fsm:changeState(State9.Npc.Command)
    Checkpoint.id = Checkpoint.id + 1
    segment.fsm:clearState() 
  end
end
State9.Dialogue.Start.exit = function(segment)
  DialogueSystem.close()
end
--命令fireOn
State9.Npc.Command = State:new()
State9.Npc.Command.enter = function(npc) 

end
State9.Npc.Command.execute = function(npc)

  if (Delay.isTimeUp(npc.tag,3)) then
    npc.ref:closeDialog()
    npc.fsm:clearState()    
  end
  npc.ref:getWeapon():setFireOn()
--  if (npc.text~= nil) then
--    npc.ref:showHints(npc.text)
--  end
end
State9.Npc.Command.exit = function(npc)
  npc.ref:getWeapon():setFireOff()
  npc.ref:closeDialog()
end
--混蛋
State9.Player.Balls = State:new()
State9.Player.Balls.execute = function(npc) 
  --抗议射击动作
  if (math.random(2) == 1) then
    npc.ref:getWeapon():setFireOn()
  else
    npc.ref:getWeapon():setFireOff()
  end

  --墙倒了则退出
  if ( Role[RoleIndex.wall].isDisposed) then
    npc.ref:setForward()
    CommonStates.OutForDisposal.execute(npc)
    return
  end

  if (npc.ref:isAlive() and npc.ref:getPosition().x < Role[RoleIndex.wall].ref:getPosition().x-60 + npc.lineIndex*1.5 ) then
    npc.ref:setForward()
  else
    npc.ref:stop()
    npc.ref:getWeapon():setFireOn()
  end 

  npc.getOpponents(npc,function(range,opponent)
      --projectTile
      if (opponent.type ~= RoleType.npc or npc.lineIndex ~= opponent.lineIndex or range < RangeType.rangeThreatened) then
        return
      end
      local isCollided = npc.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided and npc.ref:gotHurt(1,Sound.none)) then
        --给npc发一个消息,改变frame
        MsgDispatcher.dispatch(SEND_IMMEDIATELY,npc,opponent,MessageType.hit)
        npc.ref:setAnimation(Animate.fade)
        npc.ref:stop()
        npc.fsm:clearState()
        BallCounter = BallCounter - 1
      end
    end)

  npc.getTarget(npc,Role[RoleIndex.wall],function(range,wall)
      --projectTile
      if (range < RangeType.rangeThreatened) then
        return
      end
      local isCollided = npc.ref:checkObjectCollision(wall.ref,false)
      if (isCollided and npc.ref:getWeapon():isFiring() and math.random(5) == 1) then
        MsgDispatcher.dispatch(SEND_IMMEDIATELY,npc,wall,MessageType.hit)
      end
    end)
end
State9.Player.Balls.onMessage = function(npc, telegram)
  -- BallPositions[npc.lineIndex] = BallPositions[npc.lineIndex] -1

end
--史莱姆
State9.Npc.Slime = State:new()
State9.Npc.Slime.execute = function(npc)

  if (npc.ref:isAlive()) then
    CommonStates.OutForDisposal.execute(npc)
    npc.ref:setBackward()
  else
    npc.ref:stop()
  end

end
State9.Npc.Slime.onMessage = function(npc, telegram)
  --史莱姆
  if (telegram.message == MessageType.hit and npc.ref:gotHurt(1,Sound.none) == BodyStatus.dead ) then
    npc.ref:gotHurt(1,Sound.none)
    npc.ref:setAnimation(Animate.fade)
    npc.fsm:clearState()
    npc.ref:stop()
    SlimeCounter = SlimeCounter - 1
  end
  --子弹
  if (telegram.message == MessageType.gotShot ) then
    npc.ref:setAnimation(Animate.getToastHurt("HP-1"))
    local status = npc.ref:gotHurt(1,Sound.none)

    if (status == BodyStatus.dead) then
      SlimeInterval = 0.1
      SlimeCounter = SlimeCounter - 1
      npc.isDisposed = true
    end
  end
end
--推特
State9.Npc.Twitter = State:new() 
State9.Npc.Twitter.onMessage = function(npc, telegram)
  --子弹
  if (telegram.message == MessageType.gotShot ) then
    npc.ref:setAnimation(Animate.getToastHurt("HP-1"))
    local status = npc.ref:gotHurt(1,Sound.none)

    if (status == BodyStatus.dead) then

      TwitterCounter = TwitterCounter + 1
      if (TwitterCounter == TwitterMax) then
        local gunTwitter = {type=RoleType.robject, frame="twitter.png", pos=Vec2(3,8), onCheckTile=false,isSolid=false, allowNotify=true,animate=Animate.hovering}
        gunTwitter.fsm = Fsm:new(gunTwitter,nil,State9.Npc.GunTwitter)
        createRole(gunTwitter)
        Resh.playSound(Sound.howling)
        Role[1].ref:closeDialog()
      end
      npc.isDisposed = true
    end
  end
end
State9.Npc.GunTwitter = State:new() 
State9.Npc.GunTwitter.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.collider) then
    local  gunObject= {  count=-1,  sound=Sound.poogun,rate=0.5, frame="twitter.png",project={type=ProjectType.object, delay=3,step=Vec2(3,3)}}
    Role[1].ref:getWeapon():registerWeapon(gunObject)
    Role[1].isTwittered = true
    Role[1].isBlocked = nil
    Role[1].ref:closeDialog()
    npc.isDisposed=true
  end
end
--推墙
State9.RObject.Wall = State:new()
State9.RObject.Wall.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.gotShot ) then
    Resh.playSound(Sound.blocked1)

    if Role[1].isTwittered then
      npc.ref:gotHurt(10,Sound.blocked1)
      Joystick.ref:setString(CommandPad.wall,tostring(Hints.barWall..npc.ref.hp))
    end
  end

  if (telegram.message == MessageType.hit ) then
    Joystick.ref:setString(CommandPad.wall,tostring(Hints.barWall..npc.ref.hp))
    if (npc.ref.hp > npc.hpMax*0.3 or Role[1].isTwittered) then
      npc.ref:gotHurt(1,Sound.blocked1)
    else
      if not Role[1].isFired then
        Ref.stage:promptAchieve(AchieveIndex.Commander,PopupInfo.achieve(AchieveIndex.Commander))
      end
      Role[1].isBlocked = true
      Joystick.ref:showRocker()
      Resh.playSound(Sound.blocked2)
      AllowTwitter = true
    end
    if (npc.ref.hp < npc.hpMax * 0.4) then
      npc.ref:setFrame("wall4.png")
    elseif(npc.ref.hp < npc.hpMax * 0.6) then
      npc.ref:setFrame("wall3.png")
    elseif(npc.ref.hp < npc.hpMax * 0.8) then
      npc.ref:setFrame("wall2.png")
    end
  end
  if (npc.ref:isDead()) then
    Ref.stage:promptAchieve(AchieveIndex.BreakWall,PopupInfo.achieve(AchieveIndex.BreakWall))
    npc.isDisposed = true
    Role[RoleIndex.wangba].fsm:clearState() 
    Role[RoleIndex.flag].ref:spawn(Vec2(1,8),Vec2.ZERO)
    Resh.playSound(Sound.win)
  end
end
return State9