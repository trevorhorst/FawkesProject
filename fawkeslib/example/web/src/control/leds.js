import React from 'react';
import Switch from '@material-ui/core/Switch';
import FormLabel from '@material-ui/core/FormLabel';
import FormControl from '@material-ui/core/FormControl';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';

import API from '../api/api.js'

class ControlLed extends React.Component {

    constructor( props ) {
        super( props )

        let leds = [ false, false, false, false ]

        this.state = {
          leds: leds,
          led0Switch: false,
          led1Switch: false
        }

    }

    componentWillMount() {

    }

    componentDidMount() {

    }

    componentWillUnmount() {

    }

    handleChange( name, index ) {
      let comp = this;
      var ledsState = this.state.leds
      ledsState[ index ] = !ledsState[ index ]

      API.sendCommand( { "cmd":"led", "params":{ "id": index, "enable": ledsState[ index ]  } } )
      .then( function( result ) {
      } )
      .catch( function( reason ) {
        // Do something with the error
        console.log( reason )
      })

      this.setState( { leds: ledsState } )
    }

    render() {
        let comp = this;

        let switchItems = 
          [ 'LED0', 'LED1', 'LED2', 'LED3' ].map( ( text, index ) => (
            <FormControlLabel 
              control={
                <Switch
                  key={text}
                  checked={ comp.state.leds[ index ] }
                  onChange={ comp.handleChange.bind( comp, text, index ) }
                  inputProps={ { 'aria-label': 'secondary checkbox' } }
                />
              }
              key={text}
              label={text}
              labelPlacement="start"
            />
            ))

        return (
            <div>
              <FormControl>
                <FormLabel component="legend">
                  LED Control
                </FormLabel>
                <FormGroup>
                  {switchItems}
                </FormGroup>
              </FormControl>
            </div>
        );
    }

};

export default ControlLed;
