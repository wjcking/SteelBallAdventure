local Telegram = require("telegram")
local State = require("state")
local RObjectStates = { }

RObjectStates.resetBoard =function(tag,index)
  Role[tag].ref.isFlippedX = FlatHints[index].isFlippedX ~= nil

  if hintInfo.pos ~= nil then
    Role[tag].ref:spawn(FlatHints[index].pos,FlatHints[index].offset)
  end
end
--【必须】下陷
RObjectStates.Collapse = State:new()
RObjectStates.Collapse.enter = function(object)
  if (object.collapse == nil) then
    object.fsm:clearState()
  end
end
RObjectStates.Collapse.execute = function(object)

  if (object.ref:getDistance().y > object.collapse.distance.y) then
    object.isDisposed = true
  end
  object.ref.moveStep = object.collapse.step  
  object.ref:setDirection(object.collapse.direction)

end

--开关 framePrefix前缀 
--参数 object.frameIndex     object.frameTimes > 5 
RObjectStates.Switch = State:new()
RObjectStates.Switch.execute = function(object)
  local switchResult = object.ref:switchFrame(4,Role[1].ref)
  if (switchResult.touchedTimes > 0) then
    Luah.output("tag"..object.tag.."touchedtimes:"..switchResult.touchedTimes.."frameIndex"..switchResult.frameIndex)
  end
end

RObjectStates.Bounce = State:new()
RObjectStates.Bounce.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider) then
    object.ref:bounce(Role[1].ref)
  end

end
--气球 upward
RObjectStates.Balloon = State:new()
RObjectStates.Balloon.execute = function(object)
  object.ref:setUpward()
  if (object.ref:getPosition().y > 555) then 
    object.isDisposed = true
  end
end
--RotateOBB 注意距离如果限制的话可能造成碰撞误差
--最好设置距离远一些
RObjectStates.RotateOBB = State:new()
RObjectStates.RotateOBB.execute = function(object)
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeOutOfSight) then 
        return
      end

      local isObbCollided = object.ref:checkObb(player.ref,false)

      if (isObbCollided) then 
        MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,player,MessageType.obb)
      end
    end) 
end
RObjectStates.gotPushed = State:new()
RObjectStates.gotPushed.onMessage = function(object, telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end

end
--移动 不要加updatePosition 和 gotoDesirePosition
RObjectStates.Move = State:new()
RObjectStates.Move.execute = function(object)
  object.ref:moveObject(true)
end

RObjectStates.Knocked = State:new()
RObjectStates.Knocked.execute = function(object)
  object.ref:checkKnocks(Role[1].ref)
end
--敲
RObjectStates.Knocked.onMessage = function(object, telegram)

  if (telegram.message == MessageType.knocked) then
    object.ref:gotKnocked(telegram.sender.ref)
    --  local knocked =Luah.output("[RObjectStates.Knocked.onMessage]".."action:"..knocked.knockAction.."times:"..knocked.times)
  end

end

--闪烁群组
RObjectStates.BlinkGroup = State:new()
RObjectStates.BlinkGroup.execute = function(object)
  object.ref:blinkGroup()
end

RObjectStates.ProjectTile = State:new()
RObjectStates.ProjectTile.execute = function(object)
  if ((object.ref:getProjectType() == ProjectType.deadshot or object.ref:getProjectType() == ProjectType.trace ) and object.ref:isDestiny()) then
    --注意有的子弹没有target，直接获取可能无法转换
    local tag = object.ref:getTarget():getTag()
    MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,Role[tag],MessageType.hit)
    object.isDisposed = true
  end
  if (object.ref:isDead()) then
    object.isDisposed = true
  end
end
--伸缩
RObjectStates.Rebound = State:new()
RObjectStates.Rebound.execute = function(object)
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      object.ref:rebound()
    end)
end
--终点
RObjectStates.Destiny =  State:new() 
RObjectStates.Destiny.execute = function(object)
  -- 如果不是玩么都不做,前期只判断玩家
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeThreatened) then 
        return
      end
      local isCollided = object.ref:checkObjectCollision(player.ref,false)
      if (isCollided) then
        Role[1].ref:freeze()
        if (Delay.isTimeUp(1, 1.5)) then
          Ref.stage:prompt(PopupInfo.passed())
          object.fsm:clearState()
        end
      end
    end)
end

RObjectStates.DialogBoard = State:new()
RObjectStates.DialogBoard.execute = function(object)

  if (object.ref:getTouchTimes() > 0 and object.touchedText ~= nil) then 
    object.ref:registerText(object.touchedText) 
  end

  if (object.replacedText ~= nil and Role[1].ref:isDead()) then
    object.ref:registerText(object.replacedText)
    object.fsm:clearState()
  end
end
RObjectStates.Stars = State:new()
RObjectStates.Stars.onMessage = function(object, telegram)
  if (telegram.message == MessageType.collider) then

    if object.star == "Star1" then
      Ref.stage:promptAchieve(AchieveIndex.Star,PopupInfo.achieve(AchieveIndex.Star))
    end
    Resh.playSound( Sound.star)
    object.ref:stopAnimation()
    Resh.setNumber(object.star,1) 
    object.isDisposed = true
  end
end

RObjectStates.Cloud = State:new()
RObjectStates.Cloud.onMessage = function(object, telegram)
  if (telegram.message == MessageType.hit) then
    object.ref:setFrame("cloud12.png")
  end
end
RObjectStates.FlatBoard = State:new()
RObjectStates.FlatBoard.execute = function(object)
  if nil == FlatHints then
    return
  end
  if Role[1].ref:isDead() and FlatHintLost ~= nil  then
    object.ref:showHints(FlatHintLost[object.hintIndex])
    return
  end
  object.getPlayer(object,function(range,player)
      if (range < RangeType.rangeEndangered) then 
        object.ref:closeDialog()
        return
      end
      local hintInfo = FlatHints[1] 
      if object.hintIndex == nil then 
        hintInfo = FlatHints[1] 
      else
        hintInfo = FlatHints[object.hintIndex] 
      end

      object.ref:showHints(hintInfo)
    end)


end
RObjectStates.FlatBoard.onMessage = function(object, telegram)

end

return RObjectStates
