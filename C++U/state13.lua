local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State13 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}

State13.Triggers.Trap = State:new()
State13.Triggers.Trap.execute = function(trigger)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  --************左侧****************
  if  (bound.x == 3 or bound.x==4) and bound.y==10 then
    getStage():removeTileRange(Vec2(3,9),Vec2(4,9))

  end
  if trigger.isLeftSpike == nil and (bound.x == 1 or bound.x==2) and bound.y==9 then
    getStage():setGidRange(GidFlat.spikeRed,Vec2(1,9),Vec2(2,9))
    trigger.isLeftSpike = true
  end

  if trigger.isLeftSlime == nil and (bound.x == 3 or bound.x==4) and bound.y==7 then
--  Ref.stage:promptAchieve(AchieveIndex.PlayerFall4,PopupInfo.achieve(AchieveIndex.PlayerFall4))
    Role[RoleIndex.agnail].ref:setFrame("agnail_red2.png")
    Role[RoleIndex.agnail].ref:setBackward()
    trigger.isLeftSlime = true
  end
  --************右侧****************
  if trigger.isRightSpike1 == nil and (bound.x >= 14 and bound.x <=16) and bound.y==11 then
    getStage():setGidRange(GidFlat.spikeRed,Vec2(14,11),Vec2(16,11))
    trigger.isRightSpike1 = true
  end


  if trigger.isRightSpike2 == nil and (bound.x >= 13 and bound.x <=15) and bound.y==9 then
    getStage():setGidRange(GidFlat.spikeRed,Vec2(13,9),Vec2(15,9))
    trigger.isRightSpike2 = true
  end

  if trigger.isRightSpike3 == nil and (bound.x >= 14 and bound.x <=16)  and (bound.y==6 or bound.y==7) then
    getStage():setGidRange(GidFlat.spikeRed,Vec2(14,6),Vec2(16,7))
    trigger.isRightSpike3 = true
  end
  --钥匙移除
  if trigger.isRightKey == nil and bound.x == 16  and bound.y==5 then
    getStage():removeTileRange(Vec2(12,5),Vec2(12,6))
    --顶部中间倒刺移除
    getStage():removeTileRange(Vec2(6, 8),Vec2(11,9))
    --底部移除
    getStage():removeTileRange(Vec2(3,28),Vec2(4,28))
    trigger.isRightKey = true
    Role[RoleIndex.keyRight].isDisposed = true
    Role[RoleIndex.spiralBottomLeft].fsm:clearState()
    Resh.playSound(Sound.enigma)
    Role[RoleIndex.flatBoard].hintIndex = 5
    Role[RoleIndex.flatBoard].resetBoard(5)
  end
  --****************最后水晶位置*********
  if trigger.allowZOrder == nil and (bound.x == 3 or bound.x==4) and bound.y==28 then
    Role[1].ref:setZOrder(2)
    trigger.allowZOrder =true
  end
  if (bound.x == 3 or bound.x==4) and bound.y==31 then
    getStage():setGidRange(GidFlat.wallWhite,Vec2(3,28),Vec2(4,28))
    getStage():removeLayerRange(LayerCover,Vec2(3,28),Vec2(14,32))
    Resh.playSound(Sound.win)
    Role[RoleIndex.crystal].ref:stopAnimation()
    Role[RoleIndex.crystal].ref.isHarmful = false
    Role[RoleIndex.crystal].ref:spawn(Vec2(13,30),Vec2(15,15))
    trigger.fsm:clearState()
  end
end

State13.Player.Routine = State:new()
State13.Player.Routine.execute = function(player)

  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical)
  player.ref:checkSpike()
  PlayerStates.Routine.execute(player)

  local bound = player.ref:getBoundSelf().tilePosition

  if (bound.x == 8 or bound.x == 9) and bound.y >=22 and bound.y <=24 then
    player.ref:setZOrder(2)
  end
  --存档
  --if (  Checkpoint.id == 0 and bound.x >= 8 and bound.y <= 12) then
    --第一阶段
  --   Checkpoint.setPtc(Vec2(8,26))
 -- end

end
State13.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot) then
    Role[1].ref:gotHurt(1,Sound.hurt)
  end

end 
--按钮
State13.RObject.WallWhite = State:new()
State13.RObject.WallWhite.execute = function(object)
  local bound = object.ref:getBoundSelf().tilePosition

  if  bound.x == Role[RoleIndex.buttonGreen].pos.x and bound.y == Role[RoleIndex.buttonGreen].pos.y then
    object.fsm:clearState()
    return
  end
  -- 如果不是玩么都不做,前期只判断玩家
  object.getTarget(object,Role[RoleIndex.spiral2],function(range,opponent)
      if (range < RangeType.rangeThreatened) then 
        return
      end

      local isCollided = object.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided   ) then

        if (bound.x == 6 and bound.y==15) then
          object.ref:spawn(Vec2(14,15),Vec2.ZERO) 
          Resh.playSound(Sound.teleport)
        end
      end
    end)

  -- 如果不是玩么都不做,前期只判断玩家
  object.getTarget(object,Role[RoleIndex.spiral3],function(range,opponent)
      if (range < RangeType.rangeThreatened) then 
        return
      end

      local isCollided = object.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided) then
        if (bound.x == 16 and bound.y==15) then
          object.ref:spawn(Vec2(4,19),Vec2.ZERO)
          Resh.playSound(Sound.teleport)
        end
      end
    end)
end
State13.RObject.WallWhite.onMessage = function(object, telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end
--旋涡传输
State13.RObject.Teleport = State:new()
State13.RObject.Teleport.onMessage = function(object, telegram)

  if (telegram.message ~= MessageType.collider ) then
    return
  end

  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (object.tag == 2 and bound.x == 6 and bound.y==19) then
    Role[1].ref:spawn(Vec2(6,17),Vec2.ZERO)
    Resh.playSound(Sound.teleport)
  elseif (object.tag ==3 and bound.x == 6 and bound.y==15) then
    Role[1].ref:spawn(Vec2(16,17),Vec2.ZERO)
    Resh.playSound(Sound.teleport)
  elseif (object.tag == 4 and bound.x == 16 and bound.y==15) then
    Role[1].ref:spawn(Vec2(16,22),Vec2.ZERO)
    Resh.playSound(Sound.teleport)
  elseif (object.tag == 5 and bound.x == 16 and bound.y==19) then
    Role[1].ref:spawn(Vec2(4,20),Vec2.ZERO)
    Resh.playSound(Sound.teleport)
    --底部左侧传送
  elseif (object.tag == RoleIndex.spiralBottomLeft and bound.x == object.pos.x and bound.y==object.pos.y) then
    getStage():setPtc(Vec2(0,13),CameraSlideOffset)
    --恢复zorder
    Role[1].ref:setZOrder(ZOrder.role)
    --左侧隐藏区域
    if (not Role[RoleIndex.keyLeft].isDisposed) then
      Role[RoleIndex.flatBoard].hintIndex = 3
      Role[RoleIndex.flatBoard].resetBoard(3)
      getStage():removeLayerRange(LayerCover,Vec2(1,5),Vec2(4,12))
      Role[1].ref:spawn(Vec2(4,12),Vec2.ZERO) 
    else 
      --存档 --第二阶段 右侧侧隐藏区域
      Checkpoint.setPtc(Vec2(16,12))
      getStage():setPtc(Vec2(0,13),CameraSlideOffset)
      Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
      getStage():removeLayerRange(LayerCover,Vec2(13,5),Vec2(16,12))
    end
    Resh.playSound(Sound.teleport)
  elseif (object.tag == RoleIndex.spiralBottomRight and bound.x == object.pos.x and bound.y==object.pos.y) then
    Role[1].ref:setZOrder(ZOrder.role)
    local teleportPositions = { Vec2(6,17),Vec2(16,17),Vec2(6,22),Vec2(16,22),}
    getStage():setGidRange(GidFlat.spikeRed,Vec2(1,15),Vec2(5,22))
    getStage():setGidRange(GidFlat.spikeRed,Vec2(11,15),Vec2(15,22))
    Role[1].ref:spawn(Vec2(8,26),Vec2(20,0))
    Resh.playSound(Sound.teleport)
  end

end 
--按钮
State13.RObject.Button = State:new()
State13.RObject.Button.execute = function(object)
  object.getOpponents(object,function(range,opponent)
      --attention flatboard
      if opponent.tag == RoleIndex.flatBoard or object.type == RoleType.npc or range < RangeType.rangeThreatened then 
        return
      end

      object.switchResult = object.ref:switchFrame(opponent.ref)

      --armeabi gcc 不能用父类FrameIndexes注册lua所以声明此if (switchResult.frameIndex > 1) then
      if (object.switchResult:getIndex() <=1 ) then
        return
      end
      --************************第一区域，解密成功*********************
      if  Role[RoleIndex.buttonRed].switchResult:getIndex() == 2 and 
      Role[RoleIndex.buttonGreen].switchResult:getIndex() == 2 and
      Role[RoleIndex.buttonBlue].switchResult:getIndex() == 2  then

        --第一阶段存档
        if Checkpoint.id == 0 then
          Checkpoint.setPtc(Vec2(8,26))
        end
        Ref.stage:promptAchieve(AchieveIndex.Formation,PopupInfo.achieve(AchieveIndex.Formation))
        Role[RoleIndex.flatBoard].hintIndex = 2
        Role[RoleIndex.flatBoard].resetBoard(2)

        getStage():removeTileRange(Vec2(10,21),Vec2(10,22))
        Role[RoleIndex.buttonRed].isDisposed = true
        Role[RoleIndex.buttonGreen].isDisposed = true
        Role[RoleIndex.buttonBlue].isDisposed = true

        Triggers[1].fsm:changeState(State13.Triggers.Trap)

        -- Role[RoleIndex.doubleSpikes].fsm:changeState(State13.RObject.DoubleSpikes)
        Role[RoleIndex.spiralBottomLeft].fsm:changeState(State13.RObject.Teleport)
        Role[RoleIndex.spiralBottomRight].fsm:changeState(State13.RObject.Teleport)
        Role[RoleIndex.keyLeft].fsm:changeState(State13.RObject.Key)
        Role[RoleIndex.crystal].fsm:changeState(State13.RObject.CrystalShard)

        return
      end
      if  RoleIndex.buttonRed == object.tag  then
        if not  Role[RoleIndex.spikeLeft1].fsm:isInState(State13.RObject.SpikeLeft1)  then
          Role[RoleIndex.spikeLeft1].fsm:changeState(State13.RObject.SpikeLeft1)
        end
      elseif RoleIndex.buttonGreen  == object.tag and opponent.tag == RoleIndex.wallWhite then
        Role[RoleIndex.spikeLeft1].ref.moveStep = Vec2(0.2,0.2)
      elseif  RoleIndex.buttonBlue  == object.tag  then
        getStage():setGidRange(GidFlat.spikeRed,Vec2(13,20),Vec2(13,22))

      end
    end) 
end
--spikes
State13.RObject.SpikeLeft1 = State:new()
State13.RObject.SpikeLeft1.execute = function(object)
  object.ref:setForward()
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if  object.ref:getDistance().x >= object.distance.x   then
    object.ref:stop()
    object.fsm:clearState()
  end
end
State13.RObject.SpikeLeft2 = State:new()
State13.RObject.SpikeLeft2.execute = function(object)
  if (object.isDisposed) then
    object.fsm:clearState()
    return
  end 
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if  object.ref:getDistance().x >= object.distance.x   then
    object.isDisposed = true
  end
end

State13.RObject.DoubleSpikes = State:new()
State13.RObject.DoubleSpikes.execute = function(object)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x == 8 or bound.x==9)  and bound.y==20 then
    object.ref:setUpward()
  end
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if  object.ref:getDistance().y >= object.distance.y   then
    object.ref:stop()
    object.fsm:clearState()
  end

end

State13.RObject.Key = State:new()
State13.RObject.Key.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider ) then
    Role[RoleIndex.flatBoard].hintIndex = 4
    Role[RoleIndex.flatBoard].resetBoard(4)
    getStage():setPtc(Vec2(0,33),CameraSlideOffset)
    Role[1].ref:spawn(Vec2(8,26),Vec2.ZERO)
    object.isDisposed = true
    Resh.playSound(Sound.objectAcquired)
  end
end 

State13.RObject.CrystalShard = State:new()
State13.RObject.CrystalShard.onMessage = function(object, telegram)
  if (telegram.message == MessageType.hit ) then

    if (object.ref.isHarmful) then
      object.ref:stopAnimation()
      Resh.playSound(Sound.sneaky)
      object.ref:setFrame("agnail_red2.png")
      Ref.stage:promptAchieve(AchieveIndex.DickSmile4,PopupInfo.achieve(AchieveIndex.DickSmile4))
    end

  end
  --胜利
  if (telegram.message == MessageType.collider ) then
    Role[1].ref:freeze()
    Ref.stage:prompt(PopupInfo.passed())
    Resh.playSound(Sound.win)
    object.fsm:clearState()
  end
end 
return State13