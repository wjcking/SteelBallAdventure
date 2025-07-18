local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State8 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}

local  gunObject= {  count=-1,  sound=Sound.toygun,rate=0.4, frame="bullet1.png",project={type=ProjectType.object, step=Vec2(7,7), allowGravity=false,}}
local  gunCrescent= {  count=-1,  sound=Sound.poogun,rate=0.4, frame="crescent.png", project={type=ProjectType.object, delay=3,range=Vec2(300,300),step=Vec2(7,7),allowGravity=false,allowThrough=true,}}
local  gunSlime= {  count=-1,  sound=Sound.poogun,rate=1, frame="bullet1.png",project={type=ProjectType.object, delay=3,range=Vec2(300,300),step=Vec2(0.5,0.5),allowGravity=false,}}
local  gunTrace= {  count=1,  sound=Sound.poogun,rate=0.8, frame="bullet2.png",project={type=ProjectType.deadshot, step=Vec2(2,2),allowGravity=false, target=1, }}
local FrameTurtle =
{
  {type = FrameStyle.idle,  rangeStart =1, rangeEnd =2,name="turtle",},
  {type = FrameStyle.jump,  rangeStart =1, rangeEnd =2,},
  {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =2,},
  {type = FrameStyle.walkUP,  rangeStart =1, rangeEnd =2,},
  {type = FrameStyle.walkDown,  rangeStart =1, rangeEnd =2,},
  {type = FrameStyle.hurt,  rangeStart =3, rangeEnd =3,},
  {type = FrameStyle.fire,  rangeStart =2, rangeEnd =2,},
  {type = FrameStyle.die,  rangeStart =4, rangeEnd =4,},
}
local ReachedCounter = 0
local WaveCounter = 1 
local disposing = 0
local distance = Vec2(ScreenWidth,ScreenHeight)
--local step=Vec2(0.8,0.8)
local step = Vec2(0.15,0.15)
local offset=Vec2(20,0)
local RisingObjects = nil 
local ActionStyle=
{
  broken = 2,
  deadshot = 4,
  solid=6,
  shot = 8,
}
--初始化
State8.initalize = function()

  RisingObjects =
  {
    --***第1波**
    { 
      {type=RoleType.robject, frame="dialog_info.png",pos=Vec2(9,16), offset=Vec2(-20,-20),step=step,onCheckTile=false, allowGravity=false,distance=distance, isHarmful=false,text=Hints.risingBoard,},
    },
    --***第2波**
    {
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(5,15),step=step,onCheckTile=false, allowGravity=false,distance=distance, offset=offset,},
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(7,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(9,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(11,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
    },

    --***第3波**ActionStyle.broken
    {
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(5,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(7,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, action=ActionStyle.broken},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(9,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(11,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, action=ActionStyle.broken},
    },

    --***第4波** action=ActionStyle.deadshot,
    {
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(5,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, action=ActionStyle.deadshot,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(7,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, action=ActionStyle.deadshot,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(9,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,action=ActionStyle.deadshot,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(11,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, action=ActionStyle.deadshot,},
    },

    --***第5波** allowTouch = true,
    {
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(5,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, allowTouch = true,},
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(7,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,  allowTouch = true,},
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(9,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, allowTouch = true,},
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(11,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, allowTouch = true,},
    },
    --***第6波**
    { {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(4,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(5,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(6,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(7,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, },
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(8,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,  },
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(9,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_red1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_red",pos=Vec2(10,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,},
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(11,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset, },
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(12,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,  },
      {type=RoleType.npc, frame="bigslime_blue1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_blue",pos=Vec2(13,15),step=step,onCheckTile=false, allowGravity=false,distance=distance,offset=offset,  },
    },
    --***第7波**  turtle1
    {
      {type=RoleType.npc, frame="turtle1.png",frameIndexes=FrameTurtle,pos=Vec2(8,15),step=step,  allowGravity=false,distance=distance,offset=Vec2(20,-20), hp=30,hpMax=30},

    },
  }
  disposing = 0
  WaveCounter = 1
  --到达最后,checkpoint
  if (Checkpoint.id > 0 ) then
    WaveCounter = 7
    Role[1].ref:spawn(Vec2(2,12),Vec2.ZERO)
  end
  ReachedCounter = 0
    Joystick.ref:setString(CommandPad.bar,RObjectText.barStar)
end
--根据slime建立气球
local function createBalloon(slime)
  --local index = math.random(2)
  local balloonPos = Vec2(slime.pos.x,slime.pos.y-1)
  local balloons =
  {    { "balloon_red.png","balloon_green.png","balloon_blue.png","balloon_blue.png", },
    { "ccb_balloon_red.png","ccb_balloon_green.png","ccb_balloon_blue.png","ccb_balloon_yellow.png",},

  }
  local hp = slime.hp
  --从小到大，hp

  if (WaveCounter == 6) then   
    hp = 4
  elseif (WaveCounter > 2 and WaveCounter < 6) then
    hp = 2
  end

  return {type=RoleType.robject, frame=balloons[1][math.random(4)],pos=balloonPos,step=step, offset=offset,onCheckTile=false,allowNotify=true,allowCollision=CollisionDirection.atTop, action = slime.action, allowTouch=slime.allowTouch,hp=hp,}
end
--生成问号，是否可以射击
--wavecounter【右面】单数 【左面】双数
local function createQuestions()
  local pos = Vec2(16,9)
  if (WaveCounter % 2 == 0) then
    pos=Vec2(1,9)
  end
  local allowTouch = false
  local allowDragX = false
  if (WaveCounter == 6) then
    allowTouch = true
    allowDragX = true
  end

  local question = {type=RoleType.robject, frame="question_blue.png",pos=pos,step=step,onCheckTile=false,allowNotify=true,isSolid=false, allowTouch = allowTouch, allowDragX= allowDragX}
  question.fsm = Fsm:new(question,nil,State8.RObject.Question)
  createRole(question)
end
local function createRising()
  if (WaveCounter > #RisingObjects) then
    print("[createRising WaveCounter out of boundary]")
    return
  end
  local eachCount = #RisingObjects[WaveCounter]

  --先添加气球在添加npc 注意 tag 顺序，相互联系
  for i=1,eachCount,1 do
    --动态创建
    local balloon = createBalloon(RisingObjects[WaveCounter][i])
    balloon.fsm = Fsm:new(balloon,nil,State8.RObject.Balloon)
    createRole(balloon)

    RisingObjects[WaveCounter][i].fsm = Fsm:new(RisingObjects[WaveCounter][i],nil,State8.Npc.RisingObject)
    createRole(RisingObjects[WaveCounter][i])
  end
end
local function checkAllowRising()
  for _,npc in ipairs(Role) do
    --npc
    if ((npc.type == RoleType.npc or npc.text~=nil) and
      not npc.isDisposed  and 
      npc.isDisposing == nil and 
      not npc.isDisposing ) then

      if (npc.ref:getPositionY() >= distance.y) then
        ReachedCounter = ReachedCounter + 1
        --气球
        Role[npc.tag-1].isDisposed = true
        npc.isDisposing = true 
      end
      --打中气球，重力开启
      if (npc.ref.allowGravity and npc.ref:getPositionY() < -50) then
        npc.isDisposing = true 
      end 
      if (npc.isDisposing) then
        disposing = disposing + 1 
      end
    end

  end
  --Luah.output("[checkAllowRising]"..disposing)
  return disposing == #RisingObjects[WaveCounter]
end


local function releaseRising()
  for _,npc in ipairs(Role) do
    if (npc.type == RoleType.npc and npc.isDisposing ) then
      npc.isDisposed = true
    end
  end
  Joystick.ref:setString(CommandPad.bar,RObjectText.bar..tostring(ReachedCounter))
end

--***********************状态机****************
State8.Player.Routine = State:new()
State8.Player.Routine.execute = function(player)
  PlayerStates.Routine.execute(player)
end
State8.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot or telegram.message == MessageType.hit) then
    player.ref:gotHurt(1,Sound.manHurt1) 
  end
end
--刷帧slime
State8.Triggers.Dispatch= State:new()
State8.Triggers.Dispatch.enter = function(trigger)
  createRising()
  createQuestions()
end
State8.Triggers.Dispatch.execute = function(trigger)  
  --超过3个史莱姆失败
  if (ReachedCounter >= 3) then
     Ref.stage:prompt(PopupInfo.createRetry())
    trigger.fsm:clearState()
  end
  --到头后，没了
  if (WaveCounter > #RisingObjects or Role[1].ref:isDead()) then
    print("[State8.Triggers.Dispatch.execute WaveCounter out of boundary]")
    trigger.fsm:clearState()
    return
  end
--每隔 秒
  if (not Delay.isTimeUp(DelayIndex.dispatch,2)) then
    return
  end
  Delay.reset(DelayIndex.dispatch)

  Joystick.ref:setString(CommandPad.bar,RObjectText.bar..tostring(ReachedCounter))
  local allowFetch = checkAllowRising()
  --下一波
  if (allowFetch) then

    Joystick.ref:setVisible(CommandPad.fire,false)
    Role[1].ref:getWeapon():unload()
    releaseRising()
    
    if (Checkpoint.id == 0 and ReachedCounter < 3 and WaveCounter == 6 and Role[1].ref:isAlive()) then
      Checkpoint.id = Checkpoint.id + 1
    end
    
    WaveCounter = WaveCounter + 1
    disposing=0
    createRising()
    createQuestions() 
  end 
end
--气球
State8.RObject.Balloon= State:new()
State8.RObject.Balloon.execute = function(object)
  object.ref:setUpward()

  if (object.allowTouch and object.ref:getTouchTimes() > 0) then 
    Resh.playSound(Sound.balloonBroken)
    --先添加气球在添加npc 注意 tag 顺序，相互联系
    MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[object.tag + 1],MessageType.notice)
    object.isDisposed = true
  end
end
State8.RObject.Balloon.onMessage = function(object, telegram)
  --铁气球，手指触摸
  if (object.allowTouch) then
    if (telegram.message == MessageType.gotShot) then
      Resh.playSound(Sound.blocked2)
    end
    return
  end
  if (telegram.message == MessageType.gotShot) then

    local status = object.ref:gotHurt(1,Sound.none)

    if (BodyStatus.dead == status) then
      --先添加气球在添加npc 注意 tag 顺序，相互联系
      MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[object.tag + 1],MessageType.notice)
      object.isDisposed = true
      Resh.playSound(Sound.balloonBroken)
    else
      Resh.playSound(Sound.blocked1)
      object.ref:setAnimation(Animate.getToastHurt("HP-1"))
    end
  end
  if (telegram.message == MessageType.collider and object.action == ActionStyle.broken) then
    Resh.playSound(Sound.balloonBroken)
    --先添加气球在添加npc 注意 tag 顺序，相互联系
    MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[object.tag + 1],MessageType.notice)
    object.isDisposed = true
  end
end
--史莱姆
State8.Npc.RisingObject= State:new()
State8.Npc.RisingObject.execute = function(npc)
  if Role[1].ref:isDead() and 
  (npc.frame=="bigslime_red1.png" or
    npc.frame=="bigslime_green1.png" or
    npc.frame=="bigslime_blue1.png" ) then
     npc.ref:setFrameIndex(FrameStyle.idle,4,4)
    npc.ref:setFrameIndex(FrameStyle.walkUP,4,4)
  
    npc.fsm:clearState()
  end
  if (not npc.ref.allowGravity)  then
    npc.ref:setUpward()
    if (npc.type == RoleType.npc) then
      npc.ref:setTowards(Role[1].ref:getPosition())
    end

    if (npc.action == ActionStyle.shot) then
      npc.ref:getWeapon():registerWeapon(gunSlime)
      npc.ref:getWeapon():shootAt(Vec2.ZERO)
    end  
    --乌龟王八
    if (npc.frame=="turtle1.png" ) then

      npc.ref:showHints(RObjectText.turtle)
      Role[1].ref:showHints(RObjectText.playerGumble)
      if (npc.ref:getPositionY() >= ScreenHeight/2) then

        npc.fsm:changeState(State8.Npc.Turtle)
      end
    end
  end
end

State8.Npc.RisingObject.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.gotShot) then
    npc.ref:gotHurt(1,Sound.manHurt1)
    --  npc.ref:stop()
    --  npc.ref.allowGravity = true
  end
  if (telegram.message == MessageType.notice) then
    npc.ref:stop()
    npc.ref.allowGravity = true
    if (npc.action == ActionStyle.deadshot and npc.ref:isAlive()) then
      npc.ref:getWeapon():registerWeapon(gunTrace)

      npc.ref:getWeapon():shootAt(Role[1].ref:getPosition())
    end
    --乌龟王八
    if (npc.frame=="turtle1.png" ) then 
      npc.fsm:changeState(State8.Npc.Turtle)
    end
  end
end


--手枪隐藏枪
State8.RObject.Question= State:new()
State8.RObject.Question.execute = function(object)
  if (WaveCounter == 5) then
      object.ref:showHints(Hints.poo)
      object.ref.isFlippedX = true
  else
    object.ref:closeDialog()
  end
  if (object.allowDragX) then
    local bound = object.ref:getBoundSelf().tilePosition
    --拖拽到目的地后，销毁除了本身的所有问号
    if object.isCombined == nil and bound.x ==16 and bound.y == 9 then
      for _,each in ipairs(Role) do
        if (each.frame == "question_blue.png" and each.tag ~= object.tag) then
         
        --  each.ref:setAllowTouch(false)
          each.isDisposed = true
        end
      end
      --放到循环外面
      object.isCombined = true
      Resh.playSound(Sound.object)
      object.ref:setAllowTouch(false)
      object.frame = "question_red.png"
      object.ref:setFrame("question_red.png")
    end
  end
end
State8.RObject.Question.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider) then
    if (WaveCounter == 5) then
      object.ref:setFrame("poo.png")
      Role[1].ref:gotoHell(Sound.manDied)
      Resh.playSound(Sound.sneaky)
      --成就
       Ref.stage:promptAchieve(AchieveIndex.DickSmile2,PopupInfo.achieve(AchieveIndex.DickSmile2))
      return
    end
    object.ref:closeDialog()
    if (object.frame == "question_red.png") then
      Role[1].ref:getWeapon():registerWeapon(gunCrescent)
    else 
      Role[1].ref:getWeapon():registerWeapon(gunObject)
    end
    Resh.playSound(Sound.objectAcquired)
    object.isDisposed = true
    Joystick.ref:setVisible(CommandPad.fire,true)

  end
end

--乌龟王八
State8.Npc.Turtle= State:new()
State8.Npc.Turtle.enter= function(npc)
  npc.ref:closeDialog()
  Role[1].ref:closeDialog()
  npc.ref:stop()
  npc.ref.allowGravity = true
  npc.ref.moveStep = Vec2(1.5,1.5)
  local  gunTurtle= {  count=-1,  sound=Sound.poogun,rate=1, frame="bullet2.png",project={type=ProjectType.object, step=Vec2(1.8,1.8),allowGravity=false, offset=Vec2(55,0), }}
  npc.ref:getWeapon():registerWeapon(gunTurtle)
  getStage():setGidRange(Gid.concreteTop,Vec2(0,13),Vec2(18,13))
  getStage():setGidRange(Gid.concreteMid,Vec2(0,14),Vec2(18,14))
end
State8.Npc.Turtle.execute = function(npc) 

  Joystick.ref:setString(CommandPad.bar,RObjectText.barDick..tostring(npc.ref.hp))


  npc.ref:patrollingX(true)
  --跳跃
  npc.ref.onJump = npc.ref.hp < npc.hpMax * 0.7
  local playerBound = Role[1].ref:getBoundSelf().tilePosition
  local npcBound = npc.ref:getBoundSelf().tilePosition
  if npc.ref.hp < npc.hpMax*0.5 and 
  npcBound.y == playerBound.y and
  (
    (npc.ref.face == MovingDirection.toLeft and Role[1].ref.face == MovingDirection.toRight) or 
  (npc.ref.face == MovingDirection.toRight and Role[1].ref.face == MovingDirection.toLeft) 
  )then
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
  end

end
State8.Npc.Turtle.onMessage = function(npc, telegram)
  if telegram.message == MessageType.gotShot and npc.ref:isAlive() then
  if (npc.ref.hp < npc.hpMax * 0.3) then
    --快速子弹
    local  gunFast= {  count=-1,  sound=Sound.poogun,rate=0.2, frame="bullet2.png",project={type=ProjectType.object, step=Vec2(4,4),allowGravity=false, offset=Vec2(55,0), }}
     npc.ref:getWeapon():registerWeapon(gunFast)
  end
  
    npc.ref:gotHurt(1,Sound.manHurt1) 
    npc.ref:setAnimation(Animate.getToastHurt("HP-1"))

    if (npc.ref.hp <= 0) then
      npc.ref:gotoHell(Sound.manDied)
      npc.ref.isHarmful = false
      npc.ref.onJump = false
      npc.ref:stop()
      npc.ref:clearFrame()
      npc.ref:setTexture(Folder.texture.."shardyellow.png")
      npc.fsm:changeState(RObjectStates.Destiny)
    end
  end
end


return State8