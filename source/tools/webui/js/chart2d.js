function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}

VisChart2d = function (containerId, chartTitle, ylabels, rangeMin, rangeMax, ylabelsRight) {
  this.dataset = new vis.DataSet();

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
            size: 2
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
            size: 2
          }
        }
      });
    }
    this.numRightYAxis = ylabelsRight.length;
  } else {
    this.numRightYAxis = 0;
  }


  var options = {
    start: 0, //vis.moment().add(-30, 'seconds'), // changed so its faster
    end: 100, //vis.moment(),
    showCurrentTime: true,
    dataAxis: {
      left: {
        range: {
          min:rangeMin, max: rangeMax
        },
        format: function(value) {
          return value.toFixed(2);
        },
        title: {
          text: chartTitle
        }
      }
    },
    legend: {left:{position:"top-left"}},
    //clickToUse:true,
    showMajorLabels:false,
    showMinorLabels:true,
    height: '400px',
    /*
    shaded: {
      orientation: 'bottom' // top, bottom
    }
    */
  };

  this.graph2d = new vis.Graph2d(containerId, this.dataset, this.groups, options);

  this.firstIteration = 0;
}

VisChart2d.prototype.update = function(samples, iteration) {
  assert(samples.length <= this.groups.length);
  assert(this.firstIteration <= iteration);
  if (this.firstIteration == 0) {
    this.firstIteration = iteration;
  }
  for (var i = 0; i < samples.length; ++i) {
    this.dataset.add([
      {x: iteration - this.firstIteration, y:samples[i], group: i}
    ]);
  }

  for (var i = this.numLeftYAxis; i < samples.length; ++i) {
    this.dataset.add([
      {x: iteration - this.firstIteration, y:samples[i], group: i}
    ]);
  }

  var now = iteration - this.firstIteration;//vis.moment();
  var range = this.graph2d.getWindow();
  var interval = range.end - range.start;
  this.graph2d.setWindow(now - interval, now, {animation: false});
}

VisChart2d.prototype.stopDrawing = function() {
  this.firstIteration = 0;
}
