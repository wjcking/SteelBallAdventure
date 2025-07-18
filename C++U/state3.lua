local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local State = require("state")

local State3 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, }

State3.Player.Routine = State:new()
State3.Player.Routine.execute = function(player)
  getStage():setCameraCenter(player.ref, CameraView.horizontal,false)
  player.ref:limiteScreenBorder(BorderLimited.limiteHorizon)
  player.ref:checkSpike()
  Joystick.ref:setVisible(CommandPad.drop, player.ref:isCarrying())
  PlayerStates.Routine.execute(player)
  --checkpoint存档
  local bound = player.ref:getBoundSelf().tilePosition
  if (Checkpoint.id == 0 and bound.x >= 30 ) then
    Checkpoint.setPtc(Vec2(30,14))
     Role[RoleIndex.dialogBoard].ref:registerText(RObjectText.notice2)
    Role[RoleIndex.dialogBoard].ref:spawn(Vec2(34,13),Vec2.ZERO)
  end
end
State3.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot and not player.ref:isDead()) then
    player.ref:gotoHell(Sound.playerHurt)
  end
end
--开头左侧倒刺
State3.Triggers.Spike = State:new() 
State3.Triggers.Spike.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition

  if (bound.x == 0 and bound.y == 17) then
    Role[1].ref:gotoHell(Sound.spike)
    getStage():setGid(Gid.spikeBottom,Vec2(0,17))
    trigger.fsm:clearState()
  end
  --最后2个空
  if bound.x == 45 and (bound.y == 16 or bound.y==17) then
    
    Role[1].ref:gotoHell(Sound.spike)
    Resh.playSound(Sound.sneaky)
    getStage():setGid(Gid.spike,Vec2(45,16))
    getStage():setGid(Gid.spike,Vec2(45,17))
    Ref.stage:promptAchieve(AchieveIndex.DickSmile1,PopupInfo.achieve(AchieveIndex.DickSmile1))
    trigger.fsm:clearState()
  end
end


--史莱姆
State3.Npc.BigSlime = State:new()
State3.Npc.BigSlime.execute = function(npc)
  if Role[1].ref:isDead() then
    npc.ref:setFrameIndex(FrameStyle.idle,4,4)
    npc.fsm:clearState()
    return
  end
  --桥炸了，消下去300米消失
  if (npc.ref:getDistance().y > 300) then
    npc.isDisposed = true
    return
  end
  npc.getTarget(npc,Role[RoleIndex.fruitRed],function(range,fruit)
      --注意这里，距离判断，否则不触发
      if (range < RangeType.rangeThreatened) then 
        return
      end
      --
      if (not Role[1].ref:isCarrying() and  npc.ref:checkObjectCollision(fruit.ref,false)) then 
        npc.ref:getWeapon():shootAt(Vec2.ZERO)
      Ref.stage:promptAchieve(AchieveIndex.LittleApple2,PopupInfo.achieve(AchieveIndex.LittleApple2))
      end
    end)

  npc.getPlayer(npc,function(range,player)
      
      --注意这里，距离判断，否则不触发
      if (range < RangeType.rangeOutOfSight) then 
        return
      end
      local bound = player.ref:getBoundSelf().tilePosition
      npc.ref.onJump = bound.x >= Role[RoleIndex.bigSlime].pos.x  
      --如果炸弹已经点燃，则闭嘴
      if (npc.bomb ~= nil) then
        npc.ref:closeDialog()
        return
      end
      if (npc.ref:getWeapon():isFiring()) then
        npc.ref:showHints(Hints.slimeGrumble3)
     
        return
      end
      if ( bound.x >= 23  ) then 
        npc.ref:showHints(Hints.slimeGrumble2)
      elseif ( bound.x >= 20  ) then 
        npc.ref:showHints(Hints.slimeGrumble)

      else
      end 
    end)


end
State3.Npc.BigSlime.onMessage = function(npc, telegram)

end

State3.RObject.Fall =  State:new() 
State3.RObject.Fall.execute = function(object)

  local playerBound = Role[1].ref:getBoundSelf().tilePosition

  if (playerBound.x == object.pos.x and playerBound.y < object.pos.y) then
    object.ref.isSolid = true
   -- object.ref:setGravityOn()
  --  object.allowPush = CollisionDirection.intersected
   -- object.ref:allowPush(CollisionDirection.intersected)
   -- object.fsm:changeState(RObjectStates.gotPushed)
   object.fsm:clearState()
  end
end


State3.RObject.FruitRed =  State:new() 
State3.RObject.FruitRed.execute = function(object)

  local bound = object.ref:getBoundSelf().tilePosition
  
  if Role[1].ref:isCarrying() then
    object.ref:closeDialog()
  else
    object.ref:showHints(RObjectText.appleSneak)
  end
  if (not object.allowTaken ) then
    --过了井壁子直接隐藏，否则看到会效果不好
     if (bound.y > 15 ) then
       object.ref.isVisible = false
     end
    if (bound.y >= 16 ) then
      Resh.playSound(Sound.enigma)
      local CommonStates = require("commonstates")
      Role[RoleIndex.fruitShaking].ref.isHarmful = false
      Role[RoleIndex.fruitShaking].fsm:changeState(CommonStates.RObject.Fall)
      local bomb = { type = RoleType.robject,frame = "bomb1.png",pos = Vec2(13,12), offset=Vec2(-2,15), isSolid=false, allowTaken = true,allowGravity=true,}

      local role = createRole(bomb,Fsm:new(bomb,nil,State3.RObject.Bomb))
      --果子销毁
      
      object.isDisposed = true
      --关掉重力先，炸弹被放下后可还原重力状态
      role.ref:setGravityOff()
      return
    end
  end
  --检查是否丢到井里
  if ( (bound.x == 1 or bound.x == 2) and bound.y >= 13 and not Role[1].ref:isCarrying()) then

    object.allowTaken = false 
    object.ref:setGravity(Vec2(0,-0.0015))
    object.ref:setZOrder(-1)
    object.ref:setCheckTile(false)
  end

  if (Role[1].ref:isCarrying()) then 
    object.ref:setGravityOn() 
  end
end
State3.RObject.Bomb =  State:new() 
State3.RObject.Bomb.execute = function(object)

  --如果不携带则等待爆炸
  if (object.isLightUp) then
    -- Role[RoleIndex.bigSlime].ref:closeDialog()
    return
  end
  if (not object.allowTaken) then
    object.ref:setFrame("bomb2.png")
    MsgDispatcher.dispatch(1,object,object,MessageType.fuse,1)
    MsgDispatcher.dispatch(2,object,object,MessageType.fuse,2)
    MsgDispatcher.dispatch(3,object,object,MessageType.fuse,3)
    MsgDispatcher.dispatch(4,object,object,MessageType.fuse,4)  
    object.isLightUp = true
    Role[RoleIndex.bigSlime].bomb = true
  end
  local bound = object.ref:getBoundSelf().tilePosition
  --如果在桥上放下，则不在允许携带
  if (bound.x >= 20 and not Role[1].ref:isCarrying()) then
    object.allowTaken = false
    object.ref:gotoHell(Sound.none)
  end
end
State3.RObject.Bomb.onMessage = function(object, telegram)
  --如果不是引线点燃状态退出
  if (telegram.message ~= MessageType.fuse) then
    return
  end
  if (telegram.info == 4) then
    Resh.playSound(Sound.explosion)
    object.ref:setAnimation(Animate.explosion)

     Role[RoleIndex.dialogBoard].ref:registerText(RObjectText.notice2)
    getStage():setGidRange(0,Vec2(22,15),Vec2(23,15))
    getStage():setGidRange(0,Vec2(26,15),Vec2(28,15))

    object.isDisposed = true
    return
  end
  if (telegram.info == 1) then
    Resh.playSound(Sound.tick)
  end
  object.ref:setAnimation(Animate.getToastDelay(telegram.info,1))
end


State3.RObject.CheckpointTrap =  State:new() 
State3.RObject.CheckpointTrap.execute = function(object)

  object.getPlayer(object,function(range,player)
      --注意这里，距离判断，否则不触发
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (isCollided) then
        getStage():collapse({ rangeStart = Vec2(33,18), rangeEnd = Vec2(36,19),step=Vec2(2,2), direction=MovingDirection.toBottom,distance=Vec2(90,90), }) 
        object.isDisposed = true
      end
    end)

end
--队列存储顺序
local QueueLibrary = require("queue")
local BlockQueue   = QueueLibrary:new()

--【注意】初始化这里用 . 而不是 ： 否则出来的是地址而不是数字 
State3.RObject.Knocked = State:new()
State3.RObject.Knocked.init = function()


  BlockQueue.clear()
  BlockQueue.push(RoleIndex.block1)
  BlockQueue.push(RoleIndex.block2)
  BlockQueue.push(RoleIndex.block3)
  BlockQueue.push(RoleIndex.block4)  

end
State3.RObject.Knocked.execute = function(object)

  if ( BlockQueue:isEmpty() and  Role[1].isWrong == nil) then 
    
    --下面两个是陷阱
    getStage():removeTileRange(Vec2(45,16),Vec2(45,17))
    --上面2个方块裂开
    local corrupt = {
        rangeStart = Vec2(45,10),
        rangeEnd =Vec2(45,11),
        knocks= 
        {  
          {type=KnockAction.corrupt, direction=CollisionDirection.intersected,  sound= Sound.broken,},
        },
      } 
    getStage():registerKnocks(corrupt)
    object.fsm:clearState()
    Role[RoleIndex.block1].fsm:clearState()
    Role[RoleIndex.block2].fsm:clearState()
    Role[RoleIndex.block3].fsm:clearState()
    Role[RoleIndex.block4].fsm:clearState()
    Resh.playSound(Sound.enigma)
     Role[RoleIndex.dialogBoard].ref:registerText(RObjectText.bricks)
  end

  object.ref:checkKnocks(Role[1].ref)

end
--敲
State3.RObject.Knocked.onMessage = function(object, telegram)

  if (telegram.message == MessageType.knocked) then
    local knocked = object.ref:gotKnocked(telegram.sender.ref)
    if (knocked.direction ~=  telegram.sender.ref:getCollidedDirection()) then
      return
    end
    --不是敲击
    if (tonumber(BlockQueue.pop()) ~= object.tag) then
      local bound = Role[1].ref:getBoundSelf().tilePosition
      Role[1].isWrong = false
      if (bound.y > 15) then
    --  Luah.output("[State3.RObject.Knocked]".."peek:"..tonumber(BlockQueue:peek()))
      getStage():collapse({ rangeStart = Vec2(40,18), rangeEnd = Vec2(44,19),step=Vec2(2,2), direction=MovingDirection.toBottom,distance=Vec2(90,90), })
      else
        Role[RoleIndex.spikeBackward1].fsm:changeState(State3.RObject.Spike)
        Role[RoleIndex.spikeBackward2].fsm:changeState(State3.RObject.Spike)
      end
    end
    return true
  end

  return false
end
State3.RObject.Spike = State:new() 
State3.RObject.Spike.execute = function(object)
  object.ref:setBackward()
end

return State3