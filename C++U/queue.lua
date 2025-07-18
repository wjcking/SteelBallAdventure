
local Queue = {} 
local function queue(...)
  local items = {}
  local function isEmpty()
    return next(items) == nil
  end
  local function clear()
    items = {}
  end
  local function getLength()
    return #items
  end
  local function peek()
    return items[1]
  end

  local function push(item) 
    table.insert(items, #items+1, item) 
  end

  local function pop()
    local item = peek()
    -- Luah.output("pop tag"..item.receiver.tag.."delay"..item.dispatchTime)
    table.remove(items, 1)
    return item
  end

  return {
    isEmpty = isEmpty,
    clear = clear,
    getLength =getLength,
    push = push,
    pop = pop,
    peek = peek
  }
end
--实例化，表
function Queue:new()
  local q = queue()
  setmetatable(q, {__index = StateMachine})
  return q
end

return Queue
