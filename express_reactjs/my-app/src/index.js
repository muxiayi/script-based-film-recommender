import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';

var createReactClass = require('create-react-class');

var data = {
    labelText: "input search text",
    hinderText: "input paragraph of book or scripts",
    buttonImgSrc: "submit.png",
};


var Title = createReactClass({
    render: function() {
        return (
            <label>{this.props.labelText}</label>
        );
    }
});

//large input script box
var InputBox = createReactClass({

    handleChange: function() {
        this.props.onUserInput(
            this.refs.inputBox.value
        );
    },

    render: function() {
        return (
            <textarea
                placeholder={this.props.hinderText}
                value={this.props.inputText} 
                ref="inputBox"
                onChange={this.handleChange}>
            </textarea>
        );
    }
});

//seach engine box
var InputBox2 = createReactClass({

    handleChange: function() {
        this.props.onUserInput(
            this.refs.inputBox.value
        );
    },

    render: function() {
        return (
            <input
                placeholder={this.props.hinderText}
                value={this.props.inputText} 
                ref="inputBox"
                onChange={this.handleChange}>
            </input>
        );
    }
});

var LimitedInputBox = createReactClass({
    getInitialState: function() {
        return {
          inputText: ''
        };
    },

    handleUserInput: function(inputText) {
        this.setState({
            inputText: inputText
        });
    },

    handleClear: function() {
        this.delText();
        document.getElementById("outcome").innerHTML = "Waiting for results...";
        document.getElementById("genre").innerHTML = "Waiting for results...";
        document.getElementById("related").innerHTML = "Waiting for results...";
    },

    getText: function() {
        //return this.state.inputText;
        const tag = document.getElementById("selects").value;
        if(tag === 'please'){
            alert('Please choose the input text type');
            return;
        }
        else if(tag ===  'words'){
            var urls = 'http://localhost:3005/users/userInput?content='+this.state.inputText;
            console.log(urls); //to which we will send the request to write to txt

            fetch(urls)
            .then(function(response) {
                return response.json();
            })
            .then(function(myJson) {
                console.log(myJson);
                document.getElementById("outcome").innerHTML = myJson.content;
                document.getElementById("genre").innerHTML = "Waiting for results...";
            });
        }
        else if(tag === 'scripts'){
            var urls2 = 'http://localhost:3005/users/userInputScript';
            postData(urls2, { text:this.state.inputText } )
            .then(data => console.log(data)) // JSON from `response.json()` call
            .catch(error => console.error(error))

            function postData(url, data) {
            // Default options are marked with *
                return fetch(url, {
                body: JSON.stringify(data), // must match 'Content-Type' header
                cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
                credentials: 'same-origin', // include, same-origin, *omit
                headers: {
                'user-agent': 'Mozilla/4.0 MDN Example',
                'content-type': 'application/json'
                },
                method: 'POST', // *GET, POST, PUT, DELETE, etc.
                mode: 'cors', // no-cors, cors, *same-origin
                redirect: 'follow', // manual, *follow, error
                referrer: 'no-referrer', // *client, no-referrer
                })
                .then(response => response.json()) // parses response to JSON
                .then(function(myJson) {
                    console.log(myJson);
                    document.getElementById("outcome").innerHTML = myJson.content;
                    document.getElementById("genre").innerHTML = myJson.genre;
                    document.getElementById("related").innerHTML = myJson.words;
                });

            }
        }

    },

    setText: function(text) {
        this.setState({
            inputText: text
        });
    },

    delText: function() {
        this.setState({
            inputText: ''
        });
    },
    /*
    changeSelect: function() {
        const tag = document.getElementById("selects").value;
    },
    */

/*this is strange, when to use alert in which way... not using submit for now*/
    render: function() {
        return (
            <div>
                <h1>Film Recommendation</h1>
                <Title labelText={this.props.data.labelText} />
                <select  id="selects" onChange={this.changeSelect}>
                    <option value="please">Please Choose</option>
                    <option value="words">Enter Words</option>
                    <option value="scripts">Enter Scripts</option>
                </select>
                <button id="del" onClick={this.handleClear}> Delete </button>
                <button id="sub" onClick={this.getText}> Submit </button>

                <div id="indiv" className="left clear placeit">               
                    <p id="slogan"> Top 10 Recommendation </p>
                    <pre id="outcome"> Waiting for results... </pre>
                    <p id="slogan2"> The possible tags you may like (script mode only) </p>
                    <pre id="genre"> Waiting for results... </pre>
                    <p id="related words"> Related words with above tags (script mode only) </p>
                    <pre id="related"> Waiting for results... </pre>
                </div>
                <InputBox 
                    hinderText={this.props.data.hinderText}
                    inputText={this.state.inputText}
                    //regExp={this.props.data.regExp}
                    onUserInput={this.handleUserInput}
                />
            </div>
        );
    }
});


var LimitedInputBox2 = createReactClass({
    getInitialState: function() {
        return {
          inputText: ''
        };
    },

    handleUserInput: function(inputText) {
        this.setState({
            inputText: inputText
        });
    },

    handleClear: function() {
        this.delText();
        document.getElementById("queryresult").innerHTML = "Waiting for query result...";
    },

    getText: function() {
        var urls3 = 'http://localhost:3005/users/userInputQuery';
        postData(urls3, { text:this.state.inputText } )
        .then(data => console.log(data)) // JSON from `response.json()` call
        .catch(error => console.error(error))
        
        function postData(url, data) {
        // Default options are marked with *
            return fetch(url, {
            body: JSON.stringify(data), // must match 'Content-Type' header
            cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
            credentials: 'same-origin', // include, same-origin, *omit
            headers: {
            'user-agent': 'Mozilla/4.0 MDN Example',
            'content-type': 'application/json'
            },
            method: 'POST', // *GET, POST, PUT, DELETE, etc.
            mode: 'cors', // no-cors, cors, *same-origin
            redirect: 'follow', // manual, *follow, error
            referrer: 'no-referrer', // *client, no-referrer
            })
            .then(response => response.json()) // parses response to JSON
            .then(function(myJson) {
                console.log(myJson.content);
                var pure_text = myJson.content.replace(/<[^>]*>/g, "");
                document.getElementById("queryresult").innerHTML = pure_text;
            });

        }
    },

    setText: function(text) {
        this.setState({
            inputText: text
        });
    },

    delText: function() {
        this.setState({
            inputText: ''
        });
    },

    render: function() {
        return (
            <div>
                <InputBox2
                    hinderText={this.props.data.hinderText}
                    inputText={this.state.inputText}
                    //regExp={this.props.data.regExp}
                    onUserInput={this.handleUserInput}
                    className="left clear placeit"
                />
                <button id="delquery" onClick={this.handleClear}> Delete </button>
                <button id="sub" onClick={this.getText}> Search </button>                
                <div className="rolling" >
                    <pre id="queryresult"> Waiting for query result... </pre>
                </div>       
            </div>
        );
    }
});

ReactDOM.render(
  <div>
   <LimitedInputBox data={data} />
   <LimitedInputBox2 data={data}/>
  </div>,
  document.getElementById('root')
);
