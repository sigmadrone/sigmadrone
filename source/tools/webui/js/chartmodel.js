AltitudeViewModel = function (droneState) {
  this.altitude = this.startAltitude = this.AltitudeGps = this.startAltitudeGps = 0;
  this.prevIteration = 0;
  this.verticalSpeed = 0;
  this.verticalSpeedGps = 0;
  this.reset(droneState);
}

AltitudeViewModel.prototype.reset = function(droneState) {
  if (null != droneState) {
    this.altitude = this.startAltitude = droneState.altitude_meters;
    this.altitudeGps = this.startAltitudeGps = droneState.gps_altitude;
    this.prevIteration = droneState.iteration;
  } else {
    this.altitude = this.startAltitude = 0;
    this.altitudeGps = this.startAltitudeGps = 0;
    this.prevIteration = 0;
  }
  this.verticalSpeed = 0;
  this.verticalSpeedGps = 0;
}

AltitudeViewModel.prototype._update = function(altitude, altitudeGps, iteration, dt) {
  this.verticalSpeed = (altitude - this.altitude) / ((iteration - this.prevIteration) * dt);
  this.verticalSpeedGps = (altitudeGps - this.altitudeGps) / ((iteration - this.prevIteration) * dt);
  this.altitude = altitude;
  this.altitudeGps = altitudeGps;
  this.prevIteration = iteration;
}

AltitudeViewModel.prototype.update = function(droneState) {
  if (null == droneState) {
    return;
  }
  if (0 == this.prevIteration) {
    this.reset(droneState);
    return;
  }
  this._update(droneState.altitude_meters, droneState.gps_altitude, droneState.iteration, droneState.dt/1000/1000);
}

AltitudeViewModel.prototype.relativeAltitude = function() {
  return this.altitude - this.startAltitude;
}

AltitudeViewModel.prototype.relativeAltitudeGps = function() {
  return this.altitudeGps - this.startAltitudeGps;
}
