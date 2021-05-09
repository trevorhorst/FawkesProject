import React from 'react';

import IndexPageComponent from './control/navigationtool.js';

class Main extends React.Component {

    constructor( props ) {
        super( props );

        this.state = {
        }
    }

    componentWillMount() {
    }

    componentDidMount() {
    }

    componentWillUnmount() {
    }

    render() {
        let comp = this

        return (
            <IndexPageComponent />
        );
    }
}

export default Main;
