local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local State = require("state")
local CommonStates = require("commonstates")

local State4 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, }

State4.Player.Routine = State:new()
State4.Player.Routine.execute = function(player)
  local bound = player.ref:getBoundSelf().tilePosition

  if (player.ref:isAlive()) then
    if (bound.x < 61) then
      getStage():setCameraCenter(player.ref, CameraView.horizontal,false)
    else
      getStage():moveCameraX(3,200)
    end
    player.ref:limiteScreenBorder(BorderLimited.limiteHorizon)
  end
  Joystick.ref:setVisible(CommandPad.drop, player.ref:isCarrying())
  PlayerStates.Routine.execute(player)
  --checkpoint存档
  --注意滚出去的时候已经是死亡状态不能存档

  if (Checkpoint.id <= 0 and bound.x >= 37 and not player.ref:isDead() ) then
    Checkpoint.setPtc(Vec2(38,17))
  end
end
State4.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot and not player.ref:isDead()) then
    player.ref:gotoHell(Sound.playerHurt)
  end
  --注意info是opponent
  if (telegram.message == MessageType.hit  ) then
    if (telegram.info.frame == "car1.png" or 
     telegram.info.frame == "pickup1.png" or
       telegram.info.frame == "jeep1.png" or
      telegram.info.frame == "bus1.png"  ) then
      player.ref:setAnimation(Animate.rollOut)
    end
  end
end

State4.RObject.Routine = State:new() 
State4.RObject.Routine.onMessage = function(object, telegram)
  if (telegram.message == MessageType.hit  ) then
    if (telegram.receiver.frame==  "cloud11.png" ) then
      object.ref:setFrame("cloud12.png") 
    end
  end
end
--房顶上的红色问号
State4.RObject.QuestionRed = State:new() 
State4.RObject.QuestionRed.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider and object.ref.isVisible) then
    Resh.playSound(Sound.object)
    --清除水滴,注意name属性，才可以通过进行第二阶段
    for _,drip in ipairs(Role) do
      if (drip.name == "drip") then

        --清除水滴fsm，如果水滴创建则销毁
        drip.isDisposed = true
      end
      -- end
      Triggers[RoleIndex.drip].fsm:clearState() 
      Role[RoleIndex.sign].fsm:changeState(State4.RObject.Sign)
      Triggers[RoleIndex.spawnCar1].fsm:changeState(State4.Triggers.SpawnCars)

      object.isDisposed = true
    end
  end 

end

State4.RObject.Sign = State:new() 
State4.RObject.Sign.enter = function(object)
  --云彩
  Role[RoleIndex.cloud].fsm:changeState(State4.RObject.CloudDrip)
  Role[RoleIndex.cloud].ref.isSolid = false
  Role[RoleIndex.cloud].ref.isHarmful = true
end
State4.RObject.Sign.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x > 12 and bound.y > 15 and   Role[1].ref:isAlive()) then

    object.ref.isHarmful = true
    object.ref:setForward()
  end
  if (bound.x < 5 and  bound.y > 15 and Role[1].ref:isAlive()) then
    object.ref.isFlippedX = true
    object.ref.isHarmful = true
    object.ref:setBackward()
  end
  --回收
  CommonStates.OutForDisposal.execute(object)
end
--第一.云彩
State4.RObject.Cloud = State:new() 
State4.RObject.Cloud.execute = function(object)
  if (object.ref:getDistance().x >= 320) then
    object.ref:stop()
    return
  end

  local playerBound = Role[1].ref:getBoundSelf().tilePosition

  if (playerBound.x == object.ref:getBoundSelf().tilePosition.x) then
    object.ref:stopAnimation()
    object.ref:setForward()
  end
end
--第二 云彩水滴
State4.RObject.CloudDrip = State:new() 
State4.RObject.CloudDrip.execute = function(object)

  local playerBound = Role[1].ref:getBoundSelf().tilePosition
  --水滴触发点
  if (playerBound.x == object.ref:getBoundSelf().tilePosition.x -2) then
    --根据云彩的pos创建
    local pos = Vec2(Role[RoleIndex.cloud].pos.x + 7,Role[RoleIndex.cloud].pos.y)
    local  drip = {type = RoleType.npc, frame="drip_gray1.png", pos = pos,gravity = Vec2(-0.055,-0.055), offset=Vec2(-10,0), distance=Vec2(250,250), onCheckTile=false }

    createRole(drip,Fsm:new(drip,nil,CommonStates.Respawn))
    object.fsm:clearState()
  end
end
State4.RObject.Ads = State:new() 
State4.RObject.Ads.execute = function(object)
  if (object.ref:getTouchTimes() > 0 ) then
    Resh.playSound(Sound.turnPage)
    object.isDisposed = true
  end
end
State4.Npc.Bastard = State:new() 
State4.Npc.Bastard.execute = function(npc)
  if (Role[RoleIndex.ad1].isDisposed and 
    Role[RoleIndex.ad2].isDisposed and
    Role[RoleIndex.ad3].isDisposed) then
    Ref.stage:promptAchieve(AchieveIndex.Ads,PopupInfo.achieve(AchieveIndex.Ads))
    --把广告揭掉后全部离开，而不是一个一个
    Role[RoleIndex.kid].fsm:changeState(State4.Npc.BastardGoback)
    Role[RoleIndex.man].fsm:changeState(State4.Npc.BastardGoback)
    Role[RoleIndex.woman].fsm:changeState(State4.Npc.BastardGoback)
    Role[RoleIndex.lady].fsm:changeState(State4.Npc.BastardGoback)
    Role[RoleIndex.cowboy].fsm:changeState(State4.Npc.BastardGoback)
    --汽车冲过来撞混蛋，延迟
    Role[RoleIndex.car2].fsm:changeState(State4.RObject.Car2)
    --红色问号
    Role[RoleIndex.questionRed].ref.isVisible = true
    Role[RoleIndex.questionRed].ref:setAnimation({ type = Anistyle.jump, height=5,duration=0.25})
    Role[RoleIndex.questionRed].fsm:changeState(State4.RObject.QuestionRed)
    return
  end

  if (npc.ref:isDead()) then
    npc.ref:closeDialog()
    return
  end
  if (npc.ref:getRange(Role[1].ref) < RangeType.rangeEndangered) then 
    npc.ref:closeDialog()
    return
  end

  Role[RoleIndex.kid].ref:showHints( Hints.kidGrumble)
  Role[RoleIndex.cowboy].ref:showHints(Hints.cowboyGrumble)
end
State4.Npc.BastardGoback= State:new() 
State4.Npc.BastardGoback.execute = function(npc)

  if (npc.ref:isDead()) then
    npc.ref:closeDialog()
    return
  end
  if (npc.framePrefix == "kid") then 
    npc.ref:showHints( Hints.kidGoback)
    npc.ref:setBackward()
  elseif (npc.framePrefix == "man") then
    npc.ref:showHints( Hints.manGoback)
    npc.ref:setBackward()
  elseif (npc.framePrefix == "lady") then
    --npc.ref:showHints( Hints.ladyGoback)
    npc.ref:setForward()
  elseif (npc.framePrefix == "woman") then
    npc.ref:showHints( Hints.womanGoback)
    npc.ref:setForward()
  elseif (npc.framePrefix == "cowboy") then
    --可以碰撞，踩在头顶调到广告栏上
    npc.ref.isSolid = true
    npc.ref:showHints( Hints.cowboyGoback)
    npc.ref:setForward()
  end

end
State4.Npc.BastardGoback.onMessage = function(npc, telegram)
--注意info是opponent
  if (telegram.message == MessageType.hit) then
    if (telegram.sender.frame ==  "car1.png" or telegram.sender.frame ==  "jeep1.png"  
      ) then
      npc.ref:closeDialog()
      Resh.playSound(Sound.hurtPoo)
      npc.ref:setAnimation(Animate.rollOut)
      npc.fsm:clearState()
    end
  end
end 

State4.RObject.Car2 = State:new() 
State4.RObject.Car2.enter = function(object)
  object.ref.isHarmful = false
  object.ref.isVisible = true 
end
State4.RObject.Car2.execute = function(object)
  --出界了就销毁
  if (object.ref:getDistance().x >= ScreenWidth * 3) then
    object.isDisposed = true
    return
  end 
  object.ref:setBackward()
  object.getOpponents(object,function(range,opponent)
      if (opponent.type == RoleType.robject or opponent.type==RoleType.player or range < RangeType.rangeThreatened ) then 
        return
      end

      local isCollided = object.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided) then 
        --给敌人发一个消息,改变frame
        --注意给自己也发一个，message最后一个参数需要
        MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,opponent,MessageType.hit,opponent)
      end
    end)
end
--State4.RObject.Car2.onMessage = function(object,telegram)
--  if (telegram.message == MessageType.collider) then
--  end
--end
--水滴触发器
State4.Triggers.Drip = State:new() 
State4.Triggers.Drip.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  --临界点
  --注意问号销毁后则不创建
  if (bound.x <= 26 or bound.x >=38 or Role[RoleIndex.questionRed].isDisposed) then
    return
  end

  local distance = Vec2(200,200)
  local gravity= Vec2(-0.025,-0.025)
  local name = "drip"--用来移除水滴
  local  drips =
  { 
    {type = RoleType.npc, frame="drip_gray1.png",name=name,pos = Vec2(31,13),gravity = gravity, distance=distance, onCheckTile=false,  },
    {type = RoleType.npc, frame="drip_gray2.png",name=name,pos = Vec2(32,13),gravity = gravity, distance=distance, onCheckTile=false,  },
    {type = RoleType.npc, frame="drip_gray1.png",name=name,pos = Vec2(33,13),gravity = gravity, distance=distance, onCheckTile=false,  },
    {type = RoleType.npc, frame="drip_gray2.png",name=name,pos = Vec2(34,13),gravity = gravity, distance=distance, onCheckTile=false,  },
    {type = RoleType.npc, frame="drip_gray1.png",name=name,pos = Vec2(35,13),gravity = gravity, distance=distance, onCheckTile=false,  },
    {type = RoleType.npc, frame="drip_gray1.png",name=name,pos = Vec2(36,13),gravity = gravity, distance=distance, onCheckTile=false,  },
  }

  createRole(drips[1],Fsm:new(drips[1],nil,CommonStates.Respawn))
  createRole(drips[2],Fsm:new(drips[2],nil,CommonStates.Respawn))
  createRole(drips[3],Fsm:new(drips[3],nil,CommonStates.Respawn))
  createRole(drips[4],Fsm:new(drips[4],nil,CommonStates.Respawn))
  createRole(drips[5],Fsm:new(drips[5],nil,CommonStates.Respawn))
  createRole(drips[6],Fsm:new(drips[6],nil,CommonStates.Respawn)) 

  --移除
  trigger.fsm:clearState()
end

--******************
--第二阶段
--******************
State4.Triggers.SpawnCars = State:new() 
State4.Triggers.SpawnCars.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (bound.x < 45) then
    return
  end
  local direction = MovingDirection.toLeft
  local step = Vec2(2,2)

  local  cars = 
  {
    {type = RoleType.robject,frame = "car1.png", framePrefix="car", frameIndexes=FrameTemplate.car,pos = Vec2(54,17), direction=direction, step=step, insetX=15, isHarmful=true,distance=Vec2(ScreenWidth * 1.65, ScreenHeight * 1.65), isSolid=false, frameSpeed=0.05,},
    {type = RoleType.robject,frame = "jeep1.png",framePrefix="jeep",frameIndexes=FrameTemplate.jeep,pos = Vec2(60,17),  direction=direction,step=step, insetX=15, isHarmful=true,distance=Vec2(ScreenWidth * 1.85, ScreenHeight * 1.85), isSolid=false, frameSpeed=0.05,},
    {type = RoleType.robject,frame = "pickup1.png",frameIndexes=FrameTemplate.pickup,framePrefix="pickup",pos = Vec2(66,17),   direction=direction, step=step, insetX=15, isHarmful=true,distance=Vec2(ScreenWidth * 2, ScreenHeight * 2), isSolid=false, frameSpeed=0.05,},
    {type = RoleType.robject,frame =  "bus1.png",frameIndexes=FrameTemplate.bus,framePrefix="bus",pos = Vec2(74,17), direction=direction, step=step, insetX=15, isHarmful=true,distance=Vec2(ScreenWidth * 2.5, ScreenHeight * 2.5), isSolid=true, frameSpeed=0.05,},
  } 
  cars[1].fsm=Fsm:new(cars[1],nil,CommonStates.Respawn) 
  cars[2].fsm=Fsm:new(cars[2],nil,CommonStates.Respawn) 
  cars[3].fsm=Fsm:new(cars[3],nil,CommonStates.Respawn) 
  cars[4].fsm=Fsm:new(cars[4],nil,CommonStates.Respawn) 

  createRole(cars[1])
  createRole(cars[2])
  createRole(cars[3])
  createRole(cars[4])
  trigger.fsm:clearState()
end
local function processCar(callback)
  for _,each in ipairs(Role) do
    --一定要判断dispose
    if (each.framePrefix=="car" or 
      each.framePrefix=="bus" or 
      each.framePrefix=="pickup" or 
      each.framePrefix=="jeep" )and not each.isDisposed and each.isCarExcepted == nil then
      callback(each)
     
     if each.framePrefix=="bus" then
        each.ref:showHints(Hints.bus)
     end
    end
  end
end
State4.RObject.TrafficLight = State:new() 
State4.RObject.TrafficLight.execute = function(object)
  object.ref:showHints(Hints.trafficLight)
  --没开解谜
  if (Role[1].ref:getBoundSelf().tilePosition.x > 56 and not Role[RoleIndex.block1].isDisposed) then
    processCar(function(each)
        object.ref:setFrame("tlight1.png")
        each.direction = MovingDirection.toLeft
        each.ref.moveStep =  Vec2(3,3)
        each.ref.isHarmful = true
      end)
    return
  end
--  if (object.ref:getTouchTimes() ~= object.touchTimes) then
--    Resh.playSound(Sound.object)
--     object.touchTimes = object.ref:getTouchTimes()
--   end
  --汽车还没有生成时候也可以点击红绿灯
  local times = object.ref:getTouchTimes() + 1 

  if (times % 3 == 0 or times == 3) then
    object.ref:setFrame("tlight3.png")  
  elseif(times % 2 == 0 or times == 2) then
    object.ref:setFrame("tlight2.png")
  else
    object.ref:setFrame("tlight1.png") 
  end

  processCar(function(each)
      if (times % 3 == 0 or times == 3) then
        each.ref.isHarmful = false
        each.ref.moveStep =  Vec2(0,0)
        object.ref:setFrame("tlight3.png") 
        --eash.ref.isSolid = false
      elseif(times % 2 == 0 or times == 2) then
        each.ref.moveStep =  Vec2(1,1)
        each.ref.isHarmful = false
        object.ref:setFrame("tlight2.png")
      else
        object.ref:setFrame("tlight1.png")
        each.ref.moveStep =  Vec2(3,3)
        each.ref.isHarmful = true
      end
    end)
end
State4.RObject.BlockChar = State:new()
State4.RObject.BlockChar.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if(Role[RoleIndex.block1].ref:getChar() == '2' and  Role[RoleIndex.block2].ref:getChar() == '0' and
    Role[RoleIndex.block3].ref:getChar() == '1' and Role[RoleIndex.block4].ref:getChar() == '8'  ) then
    Ref.stage:promptAchieve(AchieveIndex.Fortune2018,PopupInfo.achieve(AchieveIndex.Fortune2018))
    --如果被，则汽车消失 
    --出现过cpp调用错误
    processCar(function(each) 
        each.isDisposed = true
      end)
    Role[RoleIndex.block1].isDisposed = true
    Role[RoleIndex.block2].isDisposed = true
    Role[RoleIndex.block3].isDisposed = true
    Role[RoleIndex.block4].isDisposed = true
    Role[RoleIndex.tlight].fsm:clearState()
    Resh.playSound(Sound.enigma)
    --礼品掉下来
    Role[RoleIndex.gift].ref:setGravityOn()
  end
end
State4.RObject.GiftOrange = State:new()
State4.RObject.GiftOrange.execute = function(object)

  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      object.ref:showHints(Hints.poo)
      -- 如果没打开并且在礼品旁边
      Joystick.ref:setVisible(CommandPad.open, ( range > RangeType.rangeThreatened and object.frame ~= "poo.png"))
      --如果礼品已经打开
      if (object.frame == "poo.png") then
        local isCollided = object.ref:checkObjectCollision(player.ref,false)
        if (isCollided) then
          object.isDisposed = true
        end
      end
    end)
end
State4.RObject.GiftOrange.onMessage = function(object, telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end

State4.Npc.Wangba = State:new()
State4.Npc.Wangba.execute = function(npc)
  if  Role[1].ref:isDead() then
    npc.ref:showHints(Hints.laugh)
    return
  end
  local playerBound = Role[1].ref:getBoundSelf().tilePosition

  if (playerBound.x > 62 and Role[1].ref:isAlive()) then
    npc.ref:getWeapon():setFireOn()
    getStage():setGidRange(0,Vec2(62,18),Vec2(67,18))
  else
    --npc.ref:showHints(Hints.wangba)
  end
  npc.ref:showHints(Hints.wangba)
end

State4.Npc.Wangba.onMessage = function(npc,telegram)
  --注意info是opponent
  if (telegram.message == MessageType.hit) then
    if (telegram.sender.tag == RoleIndex.zzi) then
      npc.ref:setAnimation(Animate.rollOut)
      npc.fsm:clearState()
      npc.ref:closeDialog()
    end
  end
end
--注意isDisposed，否则出错
State4.Npc.Zzi = State:new()
State4.Npc.Zzi.execute = function(npc)
  if (npc.ref:isMoving()) then
    npc.ref:showHints(Hints.zzi)
  end
  local gift = Role[RoleIndex.gift]
  if (not gift.isDisposed) then

    local giftBound = Role[RoleIndex.gift].ref:getBoundSelf().tilePosition
    if (Role[RoleIndex.gift].frame == "poo.png" and giftBound.x > 60) then
      npc.ref:setBackward()
      local wangba = Role[RoleIndex.wangba]
      --消息只发出一次，用生命状态来判断，必须判断dispose
      if (not wangba.isDisposed and wangba.ref:isAlive()) then 
        Resh.playSound(Sound.hurtPoo)
        wangba.ref:gotoHell(Sound.manDied)
        MsgDispatcher.dispatch(SEND_IMMEDIATELY,npc,wangba,MessageType.hit)
        --地图留下
        Role[RoleIndex.map].ref.isVisible = true
        --你知道
        Role[RoleIndex.map].fsm:changeState(State4.RObject.Map)
      end
    end

    isCollided = npc.ref:checkObjectCollision(gift.ref,false)
    if (isCollided) then 
      gift.isDisposed = true
      Ref.stage:promptAchieve(AchieveIndex.Zzi,PopupInfo.achieve(AchieveIndex.Zzi))
    end
  end

end
State4.RObject.Map = State:new()
State4.RObject.Map.execute = function(object)
    object.fsm:changeState(RObjectStates.Destiny)
end
State4.RObject.Map.onMessage = function(object)
  if Platform_Android ==  Resh.getPlatform() then
    object.ref.face = MovingDirection.toLeft
    object.ref.isFlippedX = true
    Resh.openURL(Urls.Community)
    object.fsm:clearState()
  end
end
return State4