local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local State = require("state")
local CommonStates = require("commonstates")
local NpcStates = require("npcstates")
local State6 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, }
local function laugh(npc)
  if (Role[1].ref:isAlive()) then
    return false
  end
    if (npc.frame == "agnail_blue1.png") then
      npc.frame = "agnail_blue2.png"
    end
       if (npc.frame == "agnail_red1.png") then
      npc.frame = "agnail_red2.png"
    end
    
    npc.ref:setFrame(npc.frame)
    npc.fsm:clearState()
    return true
end
--玩家钢蛋
State6.Player.Routine = State:new()
State6.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical)

  player.ref:checkSpike()
  PlayerStates.Routine.execute(player)
  local bound = player.ref:getBoundSelf().tilePosition
  --存档
  if (Checkpoint.id == 0 and bound.y <= 20 ) then
    Checkpoint.setPtc(Vec2(8,21))
  elseif (Checkpoint.id == 1 and bound.y <= 11 ) then
    Checkpoint.setPtc(Vec2(15,10))
     Role[RoleIndex.flatBoard].hintIndex = 4
    Role[RoleIndex.flatBoard].resetBoard(4)
    --隐藏枪
    Joystick.ref:setVisible(CommandPad.fire,false)
  end

end
State6.Player.Routine.onMessage = function(player,telegram)

  if (telegram.message == MessageType.obb) then
    player.ref:gotoHell(Sound.sting)
    --player.ref:setAnimation(Animate.getToastDelay("HP-250",0.2))
  end

end
local ShootCount = 26
local SlimeLeft = 0
local CoinCount = 0
local CoinEaten = 32
local CoinAll = 32 + 9 * 2
State6.refresh = function()
  SlimeLeft = 0
  CoinCount = 0
end
State6.Npc.Routine = State:new()
State6.Npc.Routine.execute = function(npc) 
  if (CoinCount > CoinEaten) then
    npc.ref:locate(Role[1].ref:getPosition(),false)
  else if (CoinCount >= CoinAll) then

    --  npc.fsm:clearGlobal()
  end
end
State6.Npc.Routine.onMessage = function(npc,telegram)
  if (telegram.message == MessageType.gotShot and telegram.sender.type == RoleType.projectTile) then
    npc.ref:setAnimation(Animate.getToastDelay("HP-1",0.2)) 
    npc.isDisposed = true 
    SlimeLeft = SlimeLeft + 1 
    if (SlimeLeft == ShootCount) then  
      getStage():removeTileRange(Vec2(15,11),Vec2(15,12))
        Role[RoleIndex.flatBoard].hintIndex = 3
      --星星5
       createStar(5,Vec2(8,26),ZOrder.rojbect) 
  
      --检测机关移除
      Triggers[2].fsm:changeState(State6.Triggers.Check)
      Resh.playSound(Sound.enigma)
    end
  end
  if (telegram.message == MessageType.collider) then 

    Resh.playSound(Sound.coin)
    CoinCount = CoinCount + 1
    --完啦，死定了
    if (npc.triggerSaw) then
      local rotateInfo =  { type = Anistyle.rotate,anchor=Vec2(0,0.5), duration=0.5, angle=365,limitedTimes=-1,}
      Role[RoleIndex.saw].ref:stopAnimation()
      Role[RoleIndex.saw].ref:setAnimation(rotateInfo)

    end 
    --底部
    local frame = "coin.png"
    local triggerSaw = false
    local zorder = ZOrder.robject-1
    if (CoinCount == 10) then

      for i=1,8,1 do 
        if (i == 1) then
          frame = "diamond.png"
        elseif (i==2) then 
          frame = "coin.png"
          triggerSaw = true
        else
          frame = "coin.png"
          triggerSaw = false
        end
        local coin1 = { type = RoleType.robject,frame = frame,pos = Vec2(4 + i,9),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw, onCheckTile=false,isSolid=false,}
        local coin2 = { type = RoleType.robject,frame = frame,pos = Vec2(4 + i,10),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw,onCheckTile=false,isSolid=false,}
        coin1.fsm = Fsm:new(coin1,State6.Npc.Routine,nil)
        coin2.fsm = Fsm:new(coin2,State6.Npc.Routine,nil)
        createRole(coin1)
        createRole(coin2)

      end
      --顶部
    elseif (CoinCount == 24) then
    
      for i=1,7,1 do
        if (i ~= 1) then
          triggerSaw = true 
          frame = "diamond.png" 
        else
          frame = "coin.png" 
        end
        local coin1 = { type = RoleType.robject,frame = frame,pos = Vec2(3 + i,3),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw, onCheckTile=false,isSolid=false,}
        local coin2 = { type = RoleType.robject,frame = frame,pos = Vec2(3 + i,4),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw,onCheckTile=false,isSolid=false,}
        coin1.fsm = Fsm:new(coin1,State6.Npc.Routine,nil)
        coin2.fsm = Fsm:new(coin2,State6.Npc.Routine,nil)
        createRole(coin1)
        createRole(coin2)
      end

    elseif (CoinCount == 26) then
      --左侧和右侧
      for i=1,2,1 do
        triggerSaw = false
        local coin1 = { type = RoleType.robject,frame = frame,pos = Vec2(1 + i,6),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw, onCheckTile=false,isSolid=false,}
        local coin2 = { type = RoleType.robject,frame = frame,pos = Vec2(1 + i,7),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw,onCheckTile=false,isSolid=false,}
        coin1.fsm = Fsm:new(coin1,State6.Npc.Routine,nil)
        coin2.fsm = Fsm:new(coin2,State6.Npc.Routine,nil)
        createRole(coin1)
        createRole(coin2)

        if (i ~= 1) then
          triggerSaw = true
        end

        local coin3 = { type = RoleType.robject,frame = frame,pos = Vec2(13 + i,6),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw, onCheckTile=false,isSolid=false,}
        local coin4 = { type = RoleType.robject,frame = frame,pos = Vec2(13 + i,7),allowNotify=true,zorder=zorder, triggerSaw = triggerSaw,onCheckTile=false,isSolid=false,}
        coin3.fsm = Fsm:new(coin3,State6.Npc.Routine,nil)
        coin4.fsm = Fsm:new(coin4,State6.Npc.Routine,nil)
        createRole(coin3)
        createRole(coin4)
      end
      --吃完金币后除了主角外全部清除,否则会出stopAnimation错误
    elseif (CoinCount == CoinAll) then
      for i = 2,#Role,1 do
        Role[i].isDisposed = true
      end

      getStage():removeTile(Vec2(8,6))

      local shard =   {type = RoleType.robject,file=Folder.texture.."shardred.png",pos = Vec2(8,6),  isSolid=false, onCheckTile=false, animate=Animate.hoverCrystal,}
      shard.fsm = Fsm:new(shard,nil,RObjectStates.Destiny)
      createRole(shard)
      
      Ref.stage:promptAchieve(AchieveIndex.Corrupt,PopupInfo.achieve(AchieveIndex.Corrupt))
    end
    --销毁
    npc.isDisposed = true 
  end
end
end
--第一阶段和第二阶段都有
State6.Triggers.Spike = State:new()
State6.Triggers.Spike.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  --倒刺
  if (bound.x > 6 and bound.x < 14 and bound.y == 12) then
     Ref.stage:promptAchieve(AchieveIndex.PlayerFall2,PopupInfo.achieve(AchieveIndex.PlayerFall2))
    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGidRange(GidFlat.spikeRed,Vec2(7,11),Vec2(10,12))
    trigger.fsm:clearState()
  end
end

--如果敌人都消灭了则
State6.Triggers.Check = State:new()
State6.Triggers.Check.execute = function(trigger)
  
  if (ShootCount ~= SlimeLeft) then
    return
  end
 
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x == 15 and bound.y == 13) then
     Role[RoleIndex.saw].ref:setAnimation( { type = Anistyle.rotate,anchor=Vec2(0,0.5), duration=8, angle=365,limitedTimes=-1,})
      Role[RoleIndex.saw].fsm:changeState(RObjectStates.RotateOBB)
    getStage():removeTileRange(Vec2(15,11),Vec2(15,12))
    trigger.fsm:clearState()
  end

end
--第一阶段和第二阶段都有
State6.Npc.AgnailArray1 = State:new()
State6.Npc.AgnailArray1.execute = function(npc)
  if  laugh(npc) then
      return
  end
  --先移除
  -- 如果时间到或者问号碰撞，跟踪
  if ( Role[RoleIndex.questionTop].isDisposed) then
    npc.ref:stopAnimation()
    npc.ref:locate(Role[1].ref:getPosition(),false)
  end
end

State6.Npc.AgnailArray2 = State:new()
State6.Npc.AgnailArray2.execute = function(npc)
  if  laugh(npc) then
      return
  end
  
  local bound = Role[1].ref:getBoundSelf().tilePosition
  --如果没吃问号
  if (npc.rebound == nil  and bound.y == npc.pos.y and bound.x >= npc.pos.x - 2 and bound.x < npc.pos.x) then
    local param = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(-100,0), duration=0.2, durationBack=0.2, delay=0,delayBack=0, allowBack=true, sound=Folder.sound..""}
    npc.ref:setAnimation(param)
    npc.rebound = 1
  end
  --吃了问号
  if (Role[RoleIndex.questionTop].isDisposed and npc.tag == RoleIndex.agnail13  
    and bound.y == npc.pos.y and bound.x >= npc.pos.x) then
    local param = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(100,0), duration=0.2, durationBack=0.2, delay=0,delayBack=0, allowBack=true, sound=Folder.sound..""}
    npc.ref:setAnimation(param)
    npc.rebound = 1
    npc.fsm:clearState()
  end
end

State6.RObject.QuestionTop = State:new()
State6.RObject.QuestionTop.execute = function(object)
  -- 如果不是玩家，则什么都不做,前期只判断玩家
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (isCollided) then
        local questionBottom = {type = RoleType.robject,frame = "question_red.png",pos = Vec2(16,30), isSolid=false, onCheckTile=false,}
        questionBottom.fsm = Fsm:new(questionBottom,nil,State6.RObject.QuestionBottom)
        createRole(questionBottom)
        Role[RoleIndex.agnail9].fsm:clearState()
        object.isDisposed = true
      end
    end)
end
State6.RObject.QuestionBottom = State:new()
State6.RObject.QuestionBottom.execute = function(object)

  -- 如果不是玩家，则什么都不做,前期只判断玩家
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (isCollided) then
            Role[RoleIndex.flatBoard].hintIndex = 2
                Role[RoleIndex.flatBoard].resetBoard(2)
        getStage():removeTileRange(Vec2(8,21),Vec2(9,22))
        Resh.playSound(Sound.enigma)
        --归位
        for i=2,RoleIndex.agnail13,1 do
          Role[i].fsm:clearState()
          Role[i].ref:respawn()
        end
        Role[RoleIndex.questionLeft].fsm:changeState(State6.RObject.QuestionArray)
        Role[RoleIndex.questionMiddle].fsm:changeState(State6.RObject.QuestionArray)
        Role[RoleIndex.questionRight].fsm:changeState(State6.RObject.QuestionArray)
        object.isDisposed = true
      end
    end)
end

--****************
--***第二区域
--********
State6.RObject.QuestionArray = State:new()
State6.RObject.QuestionArray.execute = function(object)

  local function tracePlayer(step)
    for i=2,RoleIndex.agnail13,1 do
      Role[i].ref:stopAnimation()
      Role[i].fsm:changeState(NpcStates.Trace)
    end
    for i=15,28,1 do
      if (step ~= nil) then
        Role[i].ref.moveStep = step
      end
      Role[i].fsm:changeState(NpcStates.Trace)
    end
  end
  local function triggerTrap()
    getStage():removeTileRange(Vec2(7,11),Vec2(10,12))
    Triggers[1].fsm:changeState(State6.Triggers.Spike)
  
  end
  -- 如果不是玩家，则什么都不做,前期只判断玩家
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (not isCollided) then
        return
      end
        Resh.playSound(Sound.object)
      if (object.tag == RoleIndex.questionLeft) then
        tracePlayer(Vec2(0.5,0.5))
      end
      if (object.tag == RoleIndex.questionMiddle) then
        triggerTrap()
      end
      if (object.tag == RoleIndex.questionRight) then
        --合并成手枪后
        if (object.frame == "question_blue.png") then
          Role[RoleIndex.questionLeft].isDisposed = true
          Joystick.ref:setVisible(CommandPad.fire,true)
          local gunPlayer = {count=-1,  sound=Sound.toygun,rate=0.2, frame="bullet1.png",project={type=ProjectType.object,step=Vec2(4,4)} } 
     
          Role[1].ref:getWeapon():registerWeapon(gunPlayer)
          tracePlayer()
          triggerTrap()
        else
          Role[RoleIndex.agnailMiddle1].ref:spawn(Vec2(6,16),Vec2.ZERO)
   Role[RoleIndex.agnailMiddle2].ref:spawn(Vec2(11,16),Vec2.ZERO)
   end
      end
      object.isDisposed = true
    end)

  -- 合并
  if (object.tag == RoleIndex.questionMiddle and
    not Role[RoleIndex.questionMiddle].isDisposed and
    not Role[RoleIndex.questionRight].isDisposed) then
    object.getTarget(object,Role[RoleIndex.questionRight],function(range,target)
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(target.ref,false)
        if (isCollided) then  
          Resh.playSound(Sound.object)
          target.frame = "question_blue.png"
          target.ref:setFrame(target.frame)
          object.isDisposed = true
        end

      end)
  end
end
return State6