initLib
initGfx

bg = Sprite.new
bg.setPicture("bg.png")

cursor = Cursor.new
cursor.setPicture("cur.png")
cursor.setPos(240 - cursor.getW / 2, 136 - cursor.getH / 2)
cursor.setSensibility(40)

no_color = Hash.new
no_color["r"] = 0
no_color["g"] = 0
no_color["b"] = 0
no_color["a"] = 0

setTextBackground(no_color)

while mayPlay
  cursor.updatePos

  startDraw

  bg.draw
  cursor.draw

  drawText(0, 0, "x : " + cursor.getX.to_s)
  drawText(0, 10, "y : " + cursor.getY.to_s)

  endDraw

  sync
end

stopGfx
stopLib