/**
 * @brief Navigation tool component
 */
import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Drawer from '@material-ui/core/Drawer';
import AppBar from '@material-ui/core/AppBar';
import CssBaseline from '@material-ui/core/CssBaseline';
import Toolbar from '@material-ui/core/Toolbar';
import List from '@material-ui/core/List';
import Typography from '@material-ui/core/Typography';
import Divider from '@material-ui/core/Divider';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';

import Timer from './timer.js';
import ControlLed from './leds.js';
import ControlGpio from './gpio.js';

const drawerWidth = 240

const useStyles = makeStyles( theme => ({
  root: {
    display: 'flex',
  },
  appBar: {
    zIndex: theme.zIndex.drawer + 1,
  },
  drawer: {
    width: drawerWidth,
    flexShrink: 0,
  },
  drawerPaper: {
    width: drawerWidth,
  },
  content: {
    flexGrow: 1,
    padding: theme.spacing(3),
  },
  toolbar: theme.mixins.toolbar,
}));

class IndexPage extends React.Component {
  constructor( props ) {
    super( props );

    this.state = {
      pageSelect: 0
    }

    // Bind methods
    this.handlePageSelect = this.handlePageSelect.bind( this );
  }

  componentWillMount() {
  }

  componentDidMount() {
  }

  componentWillUnmount() {
  }

  handlePageSelect( select ) {
    this.setState( { pageSelect: select } );
  }

  render() {

    // Set component
    let comp = this;

    // List of control pages
    let listPages = [
      <ControlGpio />
      , <ControlLed />
      , <Timer />
    ]

    // Select which page we want to render 
    let renderPage = listPages[ comp.state.pageSelect ]

    const NavigationTool = () => {
      let classes = useStyles();
      // Populate drawer items, map multiple components at once
      let drawerItems =
        [ 'GPIO', 'LED', 'Timer' ].map( ( text, index ) => (
          <ListItem button key={text}>
            <ListItemText 
              primary={ text } 
              values={ index }  
              onClick={ comp.handlePageSelect.bind( comp, index ) }
              />
          </ListItem>
        ))

      // Main navigation
      let main =
        <div className={classes.root}>
          <CssBaseline />
          <AppBar position="fixed" className={classes.appBar}>
            <Toolbar>
              <Typography variant="h6" noWrap>
                Control Software
              </Typography>
            </Toolbar>
          </AppBar>
          <Drawer
            className={classes.drawer}
            variant="permanent"
            classes={{
              paper: classes.drawerPaper,
            }}
          >
            <div className={classes.toolbar} />
            <List>
              {drawerItems}
            </List>
            <Divider />
          </Drawer>
          <main className={classes.content}>
            <div className={classes.toolbar} />
            { renderPage }
          </main>
        </div>

      return main;
    }


    return (
      <NavigationTool />
    )
  }

}

// export default makeStyles(styles)(IndexPage);
export default IndexPage;
