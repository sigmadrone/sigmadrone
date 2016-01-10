function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}

VisChart2d = function (
  containerId,
  statusId,
  chartTitle,
  ylabels,
  rangeMin,
  rangeMax,
  ylabelsRight,
  eventCallback) {
  this.dataset = new vis.DataSet({type: {start: 'ISODate', end: 'ISODate' }});
  this.statusId = statusId;
  this.rangeMin = this.originalRangeMin = rangeMin;
  this.rangeMax = this.originalRangeMax = rangeMax;

  // Create the groups, number of groups is controlled by the size of the ylabels
  // array
  this.groups = new vis.DataSet();
  for (var i = 0; i < ylabels.length; ++i) {
    this.groups.add({
        id: i,
        content: ylabels[i],
        options: {
          drawPoints: {
            style: 'circle',
            size: 3
          }
      }
    });
  }
  this.numLeftYAxis = ylabels.length;

  if (null != ylabelsRight) {
    for (var i = 0; i < ylabelsRight.length; ++i) {
      this.groups.add({
        id: this.numLeftYAxis + i,
        content: ylabelsRight[i],
        options: {
          yAxisOrientation: 'right',
          drawPoints: {
            style: 'square',
            size: 3
          }
        }
      });
    }
    this.numRightYAxis = ylabelsRight.length;
  } else {
    this.numRightYAxis = 0;
  }

  this.graph2d = new vis.Graph2d(containerId, this.dataset, this.groups, this.createOptions());

  this.constructed = false;

  var self = this;
  this.graph2d.on('rangechanged', function(event) {
    if (eventCallback != null && self.constructed) {
      eventCallback(event);
    }
  });

  var zoomInFactor = 0.8;
  var zoomOutFactor = 1/zoomInFactor;
  /*
  this.graph2d.on('doubleClick', function(event) {
    if (event.what == "background") {
      self.zoomYAxis(event.value[0], zoomOutFactor * zoomOutFactor, false);
    }
  });
  */

  this.graph2d.on('click', function(event) {
    if (event.what == "background") {
      self.zoomYAxis(event.value[0], zoomInFactor, false);
    }
  });

  this.graph2d.on('contextmenu', function(props) {
    if (props.what == "background") {
      props.event.preventDefault();
      self.zoomYAxis(props.value[0], zoomOutFactor, true);
    }
  });

  this.reset();

  this.constructed = true;
}

VisChart2d.prototype.zoomYAxis = function(mediumPoint, zoomFactor, restoreOriginalIf) {
  var halfRange = (this.rangeMax-this.rangeMin)/2 * zoomFactor;
  this.rangeMin = mediumPoint - halfRange;
  this.rangeMax = mediumPoint + halfRange;
  if (restoreOriginalIf && (this.rangeMin < this.originalRangeMin || this.rangeMax > this.originalRangeMax)) {
    this.rangeMin = this.originalRangeMin;
    this.rangeMax = this.originalRangeMax;
  }
  this.graph2d.setOptions(
    { dataAxis: { left: { range: { min: this.rangeMin, max: this.rangeMax } } } }
  );
}

VisChart2d.prototype.enableDisableGroup = function(groupName, enable) {
  var i = 0;
  var groupIds = this.groups.getIds();
  for (;i < groupIds.length; ++i) {
    if (this.groups.get(groupIds[i]).content == groupName) {
      break;
    }
  }
  assert(i < groupIds.length);
  var options = { groups: { visibility: {} } };
  options.groups.visibility[i] = enable;
  this.graph2d.setOptions(options);
}

VisChart2d.prototype.createOptions = function() {
  return {
    start: vis.moment(),
    end: vis.moment() + 60 * 1000,
    showCurrentTime: true,
    dataAxis: {
      left: {
        range: {
          min: this.rangeMin, max: this.rangeMax
        },
        format: function(value) {
          return value.toFixed(3);
        },
        //title: {
        //  text: chartTitle
        //}
      },
      right: {
        format: function(value) {
          return value.toFixed(3);
        },
      }
    },
    sampling: true,
    legend: {left:{position:"top-left"}},
    //clickToUse:true,
    showMajorLabels:false,
    showMinorLabels:true,
    height: '330px',
    interpolation: true,
  };
}

VisChart2d.prototype.reset = function() {
  this.lastIteration = this.firstIteration = -1;
  this.dataset.clear();
  this.newData = [];
  this.dataId = 0;
  this.beginTimeLine = 0;
  this.graph2d.setOptions(this.createOptions());
}

VisChart2d.prototype.update = function(samples, iteration) {
  assert(this.firstIteration <= iteration);

  if (this.firstIteration < 0) {
    this.updateStatus("COLLECT");
    this.firstIteration = iteration;
    this.beginTimeLine = vis.moment();
  }

  var xData = this.beginTimeLine + (iteration - this.firstIteration) * 5.8;
  for (var i = 0; i < this.numLeftYAxis; ++i) {
    this.newData.push(
      {id: this.dataId++, x: xData, y:samples[i], group: i}
    );
  }

  for (var i = this.numLeftYAxis; i < samples.length && i < this.groups.length; ++i) {
    this.newData.push(
      {id: this.dataId++, x: xData, y: samples[i], group: i}
    );
  }

  this.lastIteration = iteration;
}

VisChart2d.prototype.draw = function() {
  this.updateStatus("DRAWING");
  if (this.newData.length > 0) {
    this.graph2d.setWindow(
      this.beginTimeLine,
      this.newData[this.newData.length-1].x,
      {animation: false});
    this.dataset.add(this.newData);
  }
  this.updateStatus("DONE");
}

VisChart2d.prototype.setRange = function(start, end) {
  this.graph2d.setWindow(start, end);
}

VisChart2d.prototype.getRange = function() {
  return this.graph2d.getWindow();
}

VisChart2d.prototype.updateStatus = function(state) {
  if (null != this.statusId) {
    if (state == "RESET") {
      $(this.statusId).text("");
    } else if (state == "COLLECT") {
      $(this.statusId).text("Collecting data...");
    } else if (state == "DRAWING") {
      $(this.statusId).text("Drawing...");
    } else if (state == "DONE") {
      $(this.statusId).text("Collected " + this.dataId / this.groups.length + " samples");
    }
  }
}
