
function qtConjugate(q) {
  return { w: q.w, x: -q.x, y: -q.y, z: -q.z };
}

function qtMultiply(q1, q2) {
  return {
    w: q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    x: q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    y: q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    z: q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
  };
}

function qtRotateVector(qt, vector) {
  var qvector = qtMultiply(qtMultiply(qt, {w: 0, x: vector[0], y: vector[1], z: vector[2]}), qtConjugate(qt));
  return [qvector.x, qvector.y, qvector.z];
}

function qt2Euler(qt) {

  var x = qt.x, y = qt.y, z = qt.z, w = qt.w;
  var ysqr = y * y;

  // roll
  var t0 = 2.0 * (w*x + y*z);
  var t1 = 1.0 - 2.0 * (x * x + ysqr);
  var roll = Math.atan2(t0,t1);

  // pitch
  var t2 = 2.0 * (w * y - z * x);
  t2 = Math.max(Math.min(t2, 1.0), -1);
  var pitch = Math.asin(t2);

  // yaw
  var t3 = 2.0 * (w * z + x*y);
  var t4 = 1.0 - 2.0 * (ysqr + z * z);
  var yaw = Math.atan2(t3, t4);

  return [-roll, -pitch, -yaw];
}

function qt2EulerDeg(qt) {
  var euler = qt2Euler(qt).map(function(x) { return x * 180.0 / Math.PI; } );
  return euler;
}
