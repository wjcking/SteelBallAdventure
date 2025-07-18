local State = require("state")
local NpcStates = { }

NpcStates.Routine = State:new()

NpcStates.Routine.execute = function(npc)
 
end
NpcStates.Routine.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.gotShot and not npc.ref:isDead()) then
    npc.ref:gotoHell(Sound.playerHurt)
  end
  --云彩强制更新frame
--  if (telegram.sender.ref:isDead()) then
--    npc.ref:gotoHell(Sound.none)
--  end
end

NpcStates.Trace = State:new()

NpcStates.Trace.execute = function(npc)
  npc.ref:locate(Role[1].ref:getPosition(),false)
end
 
--npc子弹处理
NpcStates.ProjectTile = State:new()
NpcStates.ProjectTile.execute = function(object)
  local tag = 0
  local projectType =object.ref:getProjectType()
 
  --如果是子弹或跟踪
  if (projectType == ProjectType.object or  projectType == ProjectType.trace) then
  
    -- 如果不是玩家，则什么都不做,前期只判断玩家
    object.getPlayer(object,function(range,player)
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(player.ref,false)
        if (isCollided) then
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,player,MessageType.gotShot)
          object.isDisposed = true
        end
      end)
    --如果是一枪命中
  elseif (projectType == ProjectType.deadshot) then
    --注意有的子弹没有target，直接获取可能无法转换
    tag= object.ref:getTarget():getTag()
    object.getTarget(object,Role[tag],function(range,target)
        -- 如果是玩家自己什么或子弹
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = object.ref:checkObjectCollision(target.ref,false)
        if (isCollided) then 
          MsgDispatcher.dispatch(SEND_IMMEDIATELY,object,target,MessageType.gotShot)
          object.isDisposed = true
        end
      end)
  end
  --生命周期
  if (object.ref:isDead()) then
    object.isDisposed = true
  end
end

--NpcStates.Go = State:new()

----NpcStates.Go.execute = function(enemy)
----  enemy.getOpponents(enemy,function(range,opponent)
----      if (opponent.type ~= RoleType.player) then
----        return
----      end

----      enemy.ref:scanSensor(opponent.ref)
----      enemy.ref.onJump = enemy.sensed.direction == MovingDirection.toTop
----    end)

----end
 
NpcStates.PatrollingX = State:new()	
NpcStates.PatrollingX.execute = function(npc)
  npc.ref:patrollingX(false)

end
NpcStates.WanderingX = State:new()	
NpcStates.WanderingX.execute = function(npc)
  if (npc.wanderX == nil ) then
    npc.wanderX = ScreenWidth
  end
  npc.ref:wanderingX(npc.wanderX)
end
return NpcStates