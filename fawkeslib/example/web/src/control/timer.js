import React from 'react';

import API from '../api/api.js'

class Timer extends React.Component {

  constructor( props ) {
      super( props )

    let qchrono = {}

      this.state = {
        qchrono: qchrono
      }

    this.tick = this.tick.bind( this )
  }

  tick() {
    let comp = this;
    let res = "";
    API.sendCommand( { "cmd":"qchrono" } )
    .then( function( result ) {
      comp.setState( {
        qchrono: result.result
      } )
      res = result.result.datetime
    } )
    .catch( function() {
      // Do something to handle the error - or don't       
    } )
  }

  componentDidMount() {
    this.interval = setInterval( this.tick, 500 )
  }

  componentWillUnmount() {
    clearInterval( this.interval )
  }

  render() {
    let comp = this;
    return (
      <div>
        {comp.state.qchrono.datetime}
      </div>
    );
  }

};

export default Timer;
