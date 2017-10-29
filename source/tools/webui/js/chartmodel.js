AltitudeViewModel = function (droneState) {
  this.altitude = this.startAltitude = this.AltitudeGps = this.startAltitudeGps = 0;
  this.startAltitudeAcc = 0;
  this.altitudeRaw =  0;
  this.prevIteration = 0;
  this.verticalSpeed = 0;
  this.verticalSpeedGps = 0;
  this.targetAltitude = 0;
  this.reset(droneState);
}

AltitudeViewModel.prototype.reset = function(droneState) {
  if (null != droneState) {
    this.altitude = this.startAltitude = droneState.altitude_meters;
    this.altitudeGps = this.startAltitudeGps = droneState.gps_altitude;
    this.altitudeRaw = this.startAltitudeAcc = droneState.altitude_from_baro;
    this.targetAltitude = droneState.target_altitude;
    this.prevIteration = droneState.iteration;
  } else {
    this.altitude = this.startAltitude = 0;
    this.altitudeGps = this.startAltitudeGps = 0;
    this.altitudeRaw = this.startAltitudeAcc = 0;
    this.targetAltitude = 0;
    this.prevIteration = 0;
  }
  this.verticalSpeedArr = [0,0,0];
  this.verticalSpeed = 0;
  this.verticalSpeedGps = 0;
}

AltitudeViewModel.prototype.update = function(droneState) {
  if (null == droneState) {
    return;
  }
  if (0 == this.prevIteration) {
    this.reset(droneState);
    return;
  }
  this.verticalSpeed = droneState.vertical_speed;
  this.verticalSpeedGps = (droneState.gps_altitude - this.altitudeGps) /
    ((droneState.iteration - this.prevIteration) * droneState.dt / 1000 / 1000);
  this.altitude = droneState.altitude_meters;
  this.altitudeGps = droneState.gps_altitude;
  this.altitudeRaw = droneState.altitude_from_baro;
  this.prevIteration = droneState.iteration;
  this.targetAltitude = droneState.target_altitude;
}

AltitudeViewModel.prototype.relativeAltitude = function() {
  return this.altitude - this.startAltitude;
}

AltitudeViewModel.prototype.relativeRawAltitude = function() {
  return this.altitudeRaw - this.startAltitude;
}

AltitudeViewModel.prototype.relativeTargetAltitude = function() {
  return this.targetAltitude - this.startAltitude;
}

AltitudeViewModel.prototype.relativeAltitudeGps = function() {
  return this.altitudeGps;// - this.startAltitudeGps;
}
