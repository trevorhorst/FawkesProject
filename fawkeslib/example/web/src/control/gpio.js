/* *****************************************************************************
 * This file serves as reference to the structure of a component
 * ****************************************************************************/
import React from 'react';
import Switch from '@material-ui/core/Switch';
import FormLabel from '@material-ui/core/FormLabel';
import FormControl from '@material-ui/core/FormControl';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';

import API from '../api/api.js'

class ControlGpio extends React.Component {

  constructor( props ) {
    super( props )

    var outputEnable = [ 0x00000000, 0x00000000, 0x00000000, 0x00000000 ]
    var outputDirection = [ 0x00000000, 0x00000000, 0x00000000, 0x00000000 ]

    this.state = {
      outputEnable: outputEnable
      , outputDirection: outputDirection
    }
  }

  componentWillMount() {

  }

  componentDidMount() {

  }

  componentWillUnmount() {

  }

  handleChange( bank, pin ) {
    let comp = this;
    var bankEnableState = comp.state.outputEnable
    var enable = ( bankEnableState[ bank ] & ( 1 << pin ) ) != 0
    console.log( "Enabled: " + enable )
    if( !enable ) {
      bankEnableState[ bank ] |= ( 1 << pin ) 
    } else {
      bankEnableState[ bank ] &= ~( 1 << pin ) 
    }
    this.setState( { outputEnable: bankEnableState } )
  }

  render() {
    let comp = this;

    var gpioBank = [ [], [], [], [] ];
    var gpioCol = [ [], [], [], [] ];
    var bank = 0;
    for( var bank = 0; bank < 4; bank++ ) {
      for( var pin = 0; pin < 32; pin++ ) {
        gpioBank[ bank ].push(
            <FormControlLabel 
              control={
                <Switch
                  key={ "PIN" + pin }
                  checked={ ( comp.state.outputEnable[ bank ] & ( 1 << pin ) ) != 0 }
                  onChange={ comp.handleChange.bind( comp, bank, pin ) }
                  inputProps={ { 'aria-label': 'secondary checkbox' } }
                />
              }
              key={ "PIN" + pin }
              label={ "PIN " + pin }
              labelPlacement="start"
              style={ { marginRight: 0, marginLeft: 0 } }
            />
        )
      }
      gpioCol[ bank ].push( 
        <FormControl style={{ marginRight: 35}}>
          <FormLabel component="legend">
            GPIO {bank} Control
          </FormLabel>
          <FormGroup>
            {gpioBank[ bank ]}
          </FormGroup>
        </FormControl>
      )
    }

    return (
      <div>
        { gpioCol[ 0 ] }
        { gpioCol[ 1 ] }
        { gpioCol[ 2 ] }
        { gpioCol[ 3 ] }
      </div>
    );
  }
};

export default ControlGpio;
