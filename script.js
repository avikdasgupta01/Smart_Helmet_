// let sections = document.querySelectorAll('section');
// let navlinks= document.querySelectorAll('nav links a');
// window.onscroll=()=> {
//     sections.forEach(sec =>{
//         let top = window.scrollY;
//         let offset = sec.offsetTop;
//         let height = sec.offsetHeight;
//         let id = sec.getAttribute('id');
//         if(top >= offset && top <offset+height){
//             navlinks.forEach(link =>{
//                link.classList.remove('active') ;
//                document.querySelector('nav links a 
//                [herf*'= + id+']').classList.add('active');
//         });
//     };
// });
// }

// const navLinks = document.querySelectorAll('.nav-link');

// navLinks.forEach((link) => {
//   link.addEventListener('click', (e) => {
//     e.preventDefault();

//     const targetSection = document.querySelector(e.target.getAttribute('href'));

//     targetSection.scrollIntoView({ behavior: 'smooth' });

//     navLinks.forEach((link) => link.classList.remove('active'));

//     e.target.classList.add('active');
//   });
// });

// const navLinks = document.querySelectorAll('nav navigation links a');
// const sections = document.querySelectorAll('section');

// navLinks.forEach((link) => {
//   link.addEventListener('click', (e) => {
//     e.preventDefault();
//     navLinks.forEach((l) => l.classList.remove('active'));
//     link.classList.add('active');
//     const targetSection = document.querySelector(link.getAttribute('href'));
//     targetSection.scrollIntoView({ behavior: 'smooth' });
//   });
// });

// document.addEventListener('scroll', () => {
//   let currentSection;
//   sections.forEach((section) => {
//     const sectionTop = section.offsetTop;
//     if (pageYOffset >= sectionTop) {
//       currentSection = section;
//     }
//   });
//   navLinks.forEach((link) => {
//     if (link.getAttribute('href') === `#${currentSection.id}`) {
//       link.classList.add('active');
//     } else {
//       link.classList.remove('active');
//     }
//   });
// });



  //Import the functions you need from the SDKs you need
//   


// const scroll1 = new LocomotiveScroll({
//   el: document.querySelector('#page1'),
//   smooth: true
// });

const scroll2 = new LocomotiveScroll({
  el: document.querySelector("#mainreal"),
  smooth: true
});
